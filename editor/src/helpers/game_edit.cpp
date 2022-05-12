#include <imgui.h>
#include <ImGuizmo.h>
#include <string>
#include <functional>
#include <initializer_list>

#include "game/transform.hpp"
#include "game/component_generator.hpp"

#include "renderer/light.hpp"
#include "renderer/skeletal_model.hpp"
#include "renderer/graph.hpp"

#include "resources/sound.hpp"
#include "resources/mesh.hpp"
#include "resources/skeletal_mesh.hpp"
#include "resources/skeletal_animation.hpp"

#include "helpers/game_edit.hpp"

//#define ROTATION_GUIZMO
/**
 * @brief Useful function to receive resource path from a drag drop imgui event
 * @param i_extensions The list of extension that allowed.
 * @param func The event function to process if extension is found.
 * @return true if drop a
 */
inline bool DragDropTarget(const std::initializer_list<std::filesystem::path>& i_extensions, std::filesystem::path& o_path)
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_BROWSER_ITEM"))
        {
            o_path = *static_cast<std::filesystem::path*>(payload->Data);
            std::filesystem::path dropExtension = o_path.extension();
            for (const std::filesystem::path& wantedExtension : i_extensions)
            {
                if (dropExtension == wantedExtension)
                {
                    ImGui::EndDragDropTarget();
                    return true;
                }
            }
        }

        ImGui::EndDragDropTarget();
    }

    return false;
}

bool Helpers::Edit(std::string& io_string, const char* i_label)
{
    char strCopy[256] = "";

#if WIN32
    strcpy_s(strCopy, io_string.data());
#else
    strcpy(strCopy, io_string.data());
#endif
    bool returnValue = ImGui::InputText(i_label, strCopy, 255, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
    io_string = std::string(strCopy);

    return returnValue;
}

void Helpers::Edit(Game::Transform& io_transform)
{
    Game::Transform& trs = io_transform;
    Game::ScaleLockParams& scParams = trs.GetScaleLockParameters();

    ImGui::Text("Transform");

    ImGui::Spacing();

    Vector3 pos = trs.position;
    Vector3 rot = trs.rotation;
    Vector3 sc = trs.scale;


    // Position edit
    ImGui::Text("Position");
    DragScalar("XYZ###P", pos.element, 3);

#pragma region Scale edit
    ImGui::Text("Scale");

    if (scParams.isLock && scParams.origScale == Vector3::zero)
        scParams.ratio = 0.f;

    if (DragScalar("XYZ###S", sc.element, 3, 0.f, std::numeric_limits<float>::max(),0.1f))
    {
        if (scParams.isLock)
        {
            if (sc.x != trs.scale.get().x && scParams.origScale.x != 0.f)
                scParams.ratio = sc.x / scParams.origScale.x;
            else if (sc.y != trs.scale.get().y && scParams.origScale.y != 0.f)
                scParams.ratio = sc.y / scParams.origScale.y;
            else if (scParams.origScale.z != 0.f)
                scParams.ratio = sc.z / scParams.origScale.z;
            else
                scParams.ratio = sc.x + sc.y + sc.z;
        }
    }

    if (scParams.isLock && scParams.origScale != Vector3::zero)
        sc = scParams.origScale * scParams.ratio;
    else if (scParams.isLock && scParams.origScale == Vector3::zero)
        sc = Vector3(scParams.ratio, scParams.ratio, scParams.ratio);

    ImGui::SameLine();
    ImGui::Checkbox("Lock scale", &scParams.isLock);
#pragma endregion

    // Rotation edit
    ImGui::Text("Rotation");
    ImGui::SliderAngle("x", &rot.x);
    ImGui::SliderAngle("y", &rot.y);
    ImGui::SliderAngle("z", &rot.z);


    trs.position = pos;
    trs.rotation = rot;
    trs.scale = sc;
}

void Helpers::Edit(Game::Entity& io_entity, ImGuizmo::OPERATION& i_guizmoOperation)
{
    int guizmoOperation = static_cast<int>(i_guizmoOperation);

    ImGui::RadioButton("None",  &guizmoOperation, ImGuizmo::OPERATION::BOUNDS); // Bounds is not used, so equal to NONE

    ImGui::SameLine();
    ImGui::RadioButton("Translate", &guizmoOperation, ImGuizmo::OPERATION::TRANSLATE);

#ifdef ROTATION_GUIZMO
    ImGui::SameLine();
    ImGui::RadioButton("Rotate", &guizmoOperation, ImGuizmo::OPERATION::ROTATE);
#endif

    ImGui::SameLine();
    ImGui::RadioButton("Scale", &guizmoOperation, ImGuizmo::OPERATION::SCALE);

    i_guizmoOperation = static_cast<ImGuizmo::OPERATION>(guizmoOperation);

    ImGui::Separator();

    //ImGui::Text("Entity: %s", io_entity.name.c_str());
    Edit(io_entity.name, "Name");

    ImGui::Separator();

    ImGui::Text("ID : %llu", io_entity.GetID());

    if (io_entity.parent && ImGui::Button("Unset parent"))
        io_entity.unsettingParent = true;

    // Edit transform
    Edit(io_entity.transform);

    /// EDIT COMPONENTS
    int index = 0, removeIndex = -1;
    for (const std::unique_ptr<Game::Component>& comp : io_entity.components)
    {
        ImGui::PushID(index);

        bool compEnabled = comp->enabled;
        if (!Edit(reinterpret_cast<unsigned char*>(comp.get()), comp->GetMetaData(), compEnabled))
            removeIndex = index;
        comp->enabled = compEnabled;

        ImGui::PopID();
        index++;
    }

    if (removeIndex != -1)
        io_entity.RemoveComponentAt(removeIndex);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Add Component", ImVec2(200, 0)))
        ImGui::OpenPopup("Add component.");

    /// ADD NEW COMPONENT POPUP
    ImGui::SetNextWindowSize(ImVec2(0, 200));
    if (ImGui::BeginPopup("Add component.", ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_AlwaysVerticalScrollbar))
    {
        static ImGuiTextFilter filter;

        // Set keyboard focus on filter only when window popup
        static bool firstTime = true;
        if (firstTime) ImGui::SetKeyboardFocusHere();
        firstTime = false;

        filter.Draw("Component", 180);

        for (const ClassMetaData* metaData : Game::Component::GetRegistry())
        {
            std::string name = metaData->className;
            if (!metaData->publicClass || !filter.PassFilter(name.c_str()))
                continue;

            if (ImGui::Selectable(name.c_str()))
                io_entity.AddComponent(std::unique_ptr<Game::Component>(metaData->constructor()));
        }

        ImGui::EndPopup();
    }
}

void Helpers::Edit(Engine& io_engine, const float i_menuWidth)
{
    Renderer::Light& light = io_engine.graph->light;
    ImGui::Checkbox("ToonShading", &light.toonShading);

    if (light.toonShading)
    {
        ImGui::SliderInt("Step amount", &light.stepAmount, 1, 20);
        ImGui::SliderFloat("Step size", &light.stepSize, 0.05f, 1.f);
        ImGui::SliderFloat("Spec size", &light.specSize, 0.f, 1.f);
        ImGui::Separator();
    }

    ImGui::Spacing();

    ImGui::ColorEdit3("Ambient", light.ambient.element);
    ImGui::ColorEdit3("Diffuse", light.diffuse.element);
    ImGui::ColorEdit3("Specular", light.specular.element);
    
    ImGui::Spacing();

    ImGui::SliderFloat3("Position", light.position.element, -1.f, 1.f);

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Checkbox("Cast shadows", &light.shadow);
    ImGui::Checkbox("Adaptative bias", &light.adaptativeBias);
    ImGui::SliderFloat("Shadow bias", &light.shadowBias, 0.f, 0.05f);

    ImGui::Separator();

    if (ImGui::Button("Reset light"))
    {
        light.position = { 1.f, 1.f, 1.f, 0.f };
        light.ambient = { 0.3f, 0.3f, 0.3f };
        light.diffuse = { 1.f, 1.f, 1.f };
        light.specular = { 0.5f, 0.5f, 0.5f };

        light.toonShading = true;

        light.stepAmount = 5;
        light.stepSize = 0.1;

        light.specSize = 0.1;

        light.shadow = true;
        light.adaptativeBias = false;
        light.shadowBias = 0.005f;
    }

    ImGui::Separator();

    ImGui::DragFloat("Shadow rendering range", &io_engine.renderer->shadowRange);
    ImGui::DragFloat("Shadow rendering depth", &io_engine.renderer->shadowDepth);

    static bool show = false;
    ImGui::Checkbox("Show light's depth map", &show);
    if (show)
    {
        ImVec2 windowSize = { i_menuWidth, i_menuWidth };
        ImGui::Image(reinterpret_cast<ImTextureID>(io_engine.renderer->depthMapFBO->GetDepthMap()), windowSize, ImVec2(0, 1), ImVec2(1, 0));
    }
}

void Helpers::Edit(Resources::Sound& io_sound)
{
    ImGui::Text("Sound");

    ImGui::Checkbox("Loop", &io_sound.loop);
    
    ImGui::InputText("Sound file", const_cast<char *>(io_sound.soundPath.c_str()), 64, ImGuiInputTextFlags_CharsNoBlank);

    std::filesystem::path path;
    if (DragDropTarget({".mp3", ".wav"}, path))
        io_sound.SetSound(path.string());

    if (ImGui::Button("Play"))
        io_sound.Play();

    ImGui::SameLine();
    if (ImGui::Button("Pause"))
        io_sound.Pause();

    ImGui::SameLine();
    if (ImGui::Button("Stop"))
        io_sound.Stop();

    ImGui::SliderFloat("Volume", &io_sound.volume, 0.f, 1.f);
    io_sound.SetVolume();
}

void Helpers::Edit(Renderer::Model& io_model)
{
    ImGui::Text("Model");

    std::string meshPath = io_model.mesh ? io_model.mesh->name : "none";

    if (Edit(meshPath, "Mesh file"))
        io_model.SetMeshFromFile(meshPath);

    std::filesystem::path path;
    if (DragDropTarget({".fbx", ".obj"}, path))
        io_model.SetMeshFromFile(meshPath);

    std::string texturePath = io_model.mesh ? io_model.mesh->textureName : "none";

    if (Edit(texturePath, "Texture file"))
        io_model.SetTextureToAllSubmesh(texturePath, false);

    if (DragDropTarget({".jpg", ".png"}, path))
        io_model.SetTextureToAllSubmesh(path.string(), false);
}

void Helpers::Edit(Renderer::SkeletalModel& io_skmodel)
{
    ImGui::Text("Skeletal model");

    std::string meshPath = io_skmodel.skmesh ? io_skmodel.skmesh->name : "none";

    if (Edit(meshPath, "Skeletal mesh file"))
        io_skmodel.SetSkeletalMeshFromFile(meshPath);

    std::filesystem::path path;
    if (DragDropTarget({".fbx", ".obj"}, path))
        io_skmodel.SetSkeletalMeshFromFile(path.string());

    std::string texturePath = io_skmodel.skmesh ? io_skmodel.skmesh->textureName : "none";

    if (Edit(texturePath, "Texture file"))
        io_skmodel.SetTextureToAllSubmesh(texturePath, false);

    if (DragDropTarget({".jpg", ".png"}, path))
        io_skmodel.SetTextureToAllSubmesh(path.string(), false);

    std::string animPath = io_skmodel.skpack ? io_skmodel.skpack->animationFilename : "none";

    if (Edit(animPath, "Animations file"))
        io_skmodel.SetTextureToAllSubmesh(animPath, false);

    if (DragDropTarget({".fbx"}, path))
        io_skmodel.LoadAnimationsForThis(path.string());

    if (io_skmodel.skpack)
    {
        std::vector<const char*> animationNames = {"none" };
        static const char* currentAnim = animationNames[0];
        int packSize = io_skmodel.skpack->GetPackSize();
        for (int i = 0; i < packSize; ++i)
            animationNames.emplace_back(io_skmodel.skpack->GetAnimation(i)->animationName.c_str());

        if (ImGui::BeginCombo("Animation", currentAnim))
        {
            int animationCount = animationNames.size();
            for (unsigned int comboIndex = 0; comboIndex < animationCount; ++comboIndex)
            {
                bool selected = (currentAnim == animationNames[comboIndex]);

                if (ImGui::Selectable(animationNames[comboIndex], selected))
                {
                    currentAnim = animationNames[comboIndex];
                    io_skmodel.player.UploadAnimation(comboIndex == 0 ? io_skmodel.skpack->GetAnimation(comboIndex - 1) : nullptr);
                }

                if (selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }
    }
}

bool Helpers::Edit(unsigned char* io_component, const ClassMetaData& io_metaData, bool& io_enabled)
{
    bool keepOnEntity = true;
    if (ImGui::TreeNodeEx(io_metaData.className.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Activate", &io_enabled);

        Edit(io_component, io_metaData);

        ImGui::Spacing();

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.5f, 0.5f, 1.f));
        if (ImGui::Selectable(("Remove " + io_metaData.className).c_str()))
            keepOnEntity = false;
        ImGui::PopStyleColor();

        ImGui::Spacing();

        ImGui::TreePop();
    }
    return keepOnEntity;
}

void Helpers::Edit(unsigned char* io_component, const ClassMetaData& io_metaData)
{
    if (!io_metaData.parentClassName.empty())
        Edit(io_component, *Game::Component::FindMetaData(io_metaData.parentClassName));

    for (const DataDescriptor& desc : io_metaData.descriptors)
    {
        unsigned char* componentData = io_component + desc.offset;
        desc.changed = false;

        if (desc.viewOnly) // Just print the variables
        {
            std::string fmt;
            switch (desc.dataType)
            {
                case EDataType::INT:
                    ImGui::Text("%s", ScalarDataToString<int>(desc.name, componentData, desc.count).c_str());
                    break;
                case EDataType::FLOAT:
                    ImGui::Text("%s", ScalarDataToString<float>(desc.name, componentData, desc.count).c_str());
                    break;
                case EDataType::STRING:
                    ImGui::Text("%s", (*reinterpret_cast<std::string*>(componentData)).c_str());
                    break;
                default:
                    break;
            }
            ImGui::Spacing();
        }
        else // Edit the variables
        {
            switch (desc.dataType)
            {
                case EDataType::BOOL:
                    desc.changed = ImGui::Checkbox(desc.name.c_str(), reinterpret_cast<bool*>(componentData));
                    break;
                case EDataType::INT:
                    desc.changed = DragScalar(desc.name, reinterpret_cast<int*>(componentData), desc.count, desc.range);
                    break;
                case EDataType::FLOAT:
                    desc.changed = DragScalar(desc.name, reinterpret_cast<float*>(componentData), desc.count, desc.range);
                    break;
                case EDataType::STRING:
                    desc.changed = Edit(*reinterpret_cast<std::string*>(componentData), desc.name.c_str());
                    break;
                case EDataType::SOUND:
                    Edit(*reinterpret_cast<Resources::Sound*>(componentData));
                    break;
                case EDataType::MODEL:
                    Edit(*reinterpret_cast<Renderer::Model*>(componentData));
                    break;
                case EDataType::SKELETALMODEL:
                    Edit(*reinterpret_cast<Renderer::SkeletalModel*>(componentData));
                    break;
                default:
                    break;
            }
        }
    }
}
