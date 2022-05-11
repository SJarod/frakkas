#include <imgui.h>
#include <ImGuizmo.h>
#include <string>

#include "game/transform.hpp"
#include "game/component_generator.hpp"

#include "renderer/lowlevel/camera.hpp"
#include "renderer/light.hpp"

#include "resources/sound.hpp"

#include "helpers/game_edit.hpp"


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

    ImGui::SameLine();
    ImGui::RadioButton("Rotate", &guizmoOperation, ImGuizmo::OPERATION::ROTATE);

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

void Helpers::Edit(Renderer::LowLevel::Camera& io_camera)
{
    Helpers::Edit(io_camera.transform);
    ImGui::Spacing();

    float fovy = Maths::ToDegrees(io_camera.fovY);
    DragScalar("Field of view Y", &fovy, 1, 0.f, 180.f);
    io_camera.fovY = Maths::ToRadians(fovy);

    DragScalar("Near", &io_camera.near, 1, 0.001f, 10.f, 0.005f);
    DragScalar("Far", &io_camera.far, 1, 100.f, 5000.f, 10.f);
}

void Helpers::Edit(Renderer::Light& io_light)
{
    ImGui::Checkbox("ToonShading", &io_light.toonShading);

    if (io_light.toonShading)
    {
        ImGui::SameLine();
        ImGui::Checkbox("FiveTone", &io_light.fiveTone);
        ImGui::Checkbox("Outline", &io_light.outline);
        ImGui::SliderInt("Outline size", &io_light.outlineSize, 1, 10);
    }
    else
    {
        ImGui::Spacing();

        io_light.outline = false;
        io_light.fiveTone = false;

        ImGui::ColorEdit3("Ambient", io_light.ambient.element);
        ImGui::ColorEdit3("Diffuse", io_light.diffuse.element);
        ImGui::ColorEdit3("Specular", io_light.specular.element);
    }
    
    ImGui::Spacing();

    ImGui::SliderFloat3("Position", io_light.position.element, -1.f, 1.f);

    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("Reset light"))
    {
        io_light.position = {};
        io_light.ambient = {1.0f, 1.0f, 1.0f};
        io_light.diffuse = {};
        io_light.specular = {};
    }
}

void Helpers::Edit(Resources::Sound& io_sound)
{
    ImGui::Text("Sound");

    ImGui::Checkbox("Loop", &io_sound.loop);
    
    ImGui::InputText("Sound file", const_cast<char *>(io_sound.soundPath.c_str()), 64, ImGuiInputTextFlags_CharsNoBlank);

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_BROWSER_ITEM"))
        {
            std::string pathSound = *static_cast<std::string*>(payload->Data);
            if (pathSound.find("wav") != std::string::npos || pathSound.find("mp3") != std::string::npos)
                io_sound.SetSound(pathSound);
        }

        ImGui::EndDragDropTarget();
    }

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
                    ImGui::Checkbox(desc.name.c_str(), reinterpret_cast<bool*>(componentData));
                    break;
                case EDataType::INT:
                    DragScalar(desc.name, reinterpret_cast<int*>(componentData), desc.count, desc.range);
                    break;
                case EDataType::FLOAT:
                    DragScalar(desc.name, reinterpret_cast<float*>(componentData), desc.count, desc.range);
                    break;
                case EDataType::STRING:
                    Edit(*reinterpret_cast<std::string*>(componentData), desc.name.c_str());
                    break;
                case EDataType::CAMERA:
                    Edit(*reinterpret_cast<Renderer::LowLevel::Camera*>(componentData));
                    break;
                case EDataType::SOUND:
                    Edit(*reinterpret_cast<Resources::Sound*>(componentData));
                    break;
                default:
                    break;
            }
        }
    }
}
