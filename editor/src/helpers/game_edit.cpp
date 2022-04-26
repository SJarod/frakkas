#include <imgui.h>
#include <ImGuizmo.h>
#include <string>

#include "game/transform.hpp"
#include "game/entity.hpp"
#include "game/component.hpp"

#include "renderer/lowlevel/camera.hpp"
#include "renderer/light.hpp"

#include "resources/sound.hpp"

#include "helpers/game_edit.hpp"


void Helpers::Edit(std::string& io_string, const char* i_label)
{
    char newStr[255] = "";
    memcpy(newStr, io_string.c_str(), io_string.size());
    if (ImGui::InputText(i_label, newStr, 255, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        io_string= newStr;
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
    DragScalar("Position", pos.element, 3, 0.1f);

    // Rotation edit
    ImGui::Text("Rotation");
    ImGui::SliderAngle("x", &rot.x);
    ImGui::SliderAngle("y", &rot.y);
    ImGui::SliderAngle("z", &rot.z);

#pragma region Scale edit
    if (scParams.isLock && scParams.origScale == Vector3::zero)
        scParams.ratio = 0.f;

    if (DragScalar("Scale", sc.element, 3, 0.1f))
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

    trs.position = pos;
    trs.rotation = rot;
    trs.scale = sc;
}

void Helpers::Edit(Game::Entity& io_entity, int& i_gizmoType)
{
    ImGui::RadioButton("None", &i_gizmoType, -1);

    ImGui::SameLine();
    ImGui::RadioButton("Translate", &i_gizmoType, ImGuizmo::OPERATION::TRANSLATE);

    ImGui::SameLine();
    ImGui::RadioButton("Rotate", &i_gizmoType, ImGuizmo::OPERATION::ROTATE);

    ImGui::SameLine();
    ImGui::RadioButton("Scale", &i_gizmoType, ImGuizmo::OPERATION::SCALE);

    ImGui::Separator();

    //ImGui::Text("Entity: %s", io_entity.name.c_str());
    Edit(io_entity.name, "Name");

    ImGui::Separator();

    Edit(io_entity.transform);

    for (const std::unique_ptr<Game::Component>& comp : io_entity.components)
    {
        bool compEnabled = comp->enabled;
        Edit(reinterpret_cast<unsigned char*>(comp.get()), comp->GetMetaData(), compEnabled);
        comp->enabled = compEnabled;
    }
}

void Helpers::Edit(Renderer::LowLevel::Camera& io_camera)
{
    Helpers::Edit(io_camera.transform);
    ImGui::Spacing();

    float fovy = Maths::ToDegrees(io_camera.fovY);
    DragScalar("Field of view Y", &fovy, 1, 1.f, 0.f, 180.f);
    io_camera.fovY = Maths::ToRadians(fovy);

    DragScalar("Near", &io_camera.near, 1, 0.01f, 0.001f);
    DragScalar("Far", &io_camera.far, 1, 10.f, 100.f, 5000.f);
}

void Helpers::Edit(Renderer::Light& io_light)
{
    ImGui::Text("Light");

    ImGui::Checkbox("ToonShading", &io_light.toonShading);

    if (io_light.toonShading)
    {
        ImGui::Checkbox("Outline", &io_light.outline);
        ImGui::Checkbox("FiveTone", &io_light.fiveTone);
    }
    else
    {
        io_light.outline = false;
        io_light.fiveTone = false;
    }
    
    ImGui::Spacing();

    DragScalar("Position", io_light.position.element, 3);
    ImGui::ColorEdit3("Ambient", io_light.ambient.element);
    ImGui::ColorEdit3("Diffuse", io_light.diffuse.element);
    ImGui::ColorEdit3("Specular", io_light.specular.element);

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

    if (ImGui::Button("Play"))
        io_sound.Play();

    ImGui::SameLine();
    if (ImGui::Button("Pause"))
        io_sound.Pause();

    ImGui::SameLine();
    if (ImGui::Button("Stop"))
        io_sound.Stop();

    ImGui::InputInt("Volume", &io_sound.volume);
    io_sound.SetVolume();
}

void Helpers::Edit(unsigned char* io_component, const ClassMetaData& io_metaData, bool& io_enabled)
{
    if (ImGui::TreeNodeEx(io_metaData.className.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Activate", &io_enabled);

        for (const DataDescriptor& desc : io_metaData.descriptors)
        {
            unsigned char* componentData = io_component + desc.offset;

            switch (desc.dataType)
            {
            case DataType::BOOL:
                ImGui::Checkbox(desc.name.c_str(), reinterpret_cast<bool*>(componentData));
                break;
            case DataType::INT:
                DragScalar(desc.name, reinterpret_cast<int*>(componentData), desc.count);
                break;
            case DataType::FLOAT:
                DragScalar(desc.name, reinterpret_cast<float*>(componentData), desc.count);
                break;
            case DataType::STRING:
                Edit(*reinterpret_cast<std::string*>(componentData), desc.name.c_str());
                break;
            case DataType::CAMERA:
                Edit(*reinterpret_cast<Renderer::LowLevel::Camera*>(componentData));
                break;
            case DataType::LIGHT:
                Edit(*reinterpret_cast<Renderer::Light*>(componentData));
                break;
            case DataType::SOUND:
                Edit(*reinterpret_cast<Resources::Sound*>(componentData));
                break;
            default:
                break;
            }
        }

        ImGui::TreePop();
    }
}
