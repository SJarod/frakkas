#include <imgui.h>

#include "game/transform.hpp"
#include "game/entity.hpp"
#include "game/component.hpp"

#include "renderer/lowlevel/camera.hpp"
#include "renderer/light.hpp"

#include "helpers/game_edit.hpp"



void Helpers::EditTransform(Game::Transform& io_transform)
{
    Game::Transform& trs = io_transform;
    Game::ScaleLockParams& scParams = trs.GetScaleLockParameters();

    ImGui::Text("Transform");

    ImGui::Spacing();

    Vector3 pos = trs.position;
    Vector3 rot = Maths::ToDegrees(trs.rotation);
    Vector3 sc = trs.scale;

    // Position edit
    ImGui::DragFloat3("Position", pos.element, 0.1f);

    // Rotation edit
    ImGui::DragFloat3("Rotation", rot.element, 1.f);
    for (float& coord : rot.element)
        coord = Maths::Abs(coord) >= 360.f ? 0.f : coord;

#pragma region Scale edit
    if (scParams.isLock && scParams.origScale == Vector3::zero)
        scParams.ratio = 0.f;

    if (ImGui::DragFloat3("Scale", sc.element, 0.1f))
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
    trs.rotation = Maths::ToRadians(rot);
    trs.scale = sc;
}

void Helpers::EditEntity(Game::Entity& io_entity)
{
    //ImGui::Text("Entity: %s", io_entity.name.c_str());
    char newName[255] = "";
    memcpy(newName, io_entity.name.c_str(), io_entity.name.size());
    if (ImGui::InputText("Name", newName, 255, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        io_entity.name = newName;

    ImGui::Separator();

    Helpers::EditTransform(io_entity.transform);

    for (const std::unique_ptr<Game::Component>& comp : io_entity.components)
        EditComponentMetaData(reinterpret_cast<unsigned char*>(comp.get()), comp->GetMetaData(), comp->enabled);
}

void Helpers::EditCamera(Renderer::LowLevel::Camera& io_camera)
{
    Helpers::EditTransform(io_camera.transform);
    ImGui::Spacing();

    float fovy = Maths::ToDegrees(io_camera.fovY);
    ImGui::SliderFloat("Field of view Y", &fovy, 0.f, 180.f);
    io_camera.fovY = Maths::ToRadians(fovy);

    ImGui::DragFloat("Near", &io_camera.near, 0.01f, 0.001f);
    ImGui::DragFloat("Far", &io_camera.far, 10.f, 100.f, 5000.f);
}

void Helpers::EditLight(Renderer::Light& io_light)
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

    ImGui::DragFloat4("Position", io_light.position.element);
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

void Helpers::EditComponentMetaData(unsigned char* io_component, const ClassMetaData& metaData, Property<bool>& io_enabled)
{
    if (ImGui::TreeNodeEx(metaData.className.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        bool enabled = io_enabled;
        if(ImGui::Checkbox("Activate", &enabled))
            io_enabled = enabled;

        for (const DataDescriptor& desc : metaData.descriptors)
        {
            unsigned char* componentData = io_component + desc.offset;

            switch (desc.dataType)
            {
            case DataType::BOOL:
                ImGui::Checkbox(desc.name.c_str(), reinterpret_cast<bool*>(componentData));
                break;
            case DataType::INT:
                ImGui::DragScalarN(desc.name.c_str(), ImGuiDataType_S32, componentData, desc.count);
                break;
            case DataType::FLOAT:
                ImGui::DragScalarN(desc.name.c_str(), ImGuiDataType_Float, componentData, desc.count);
                break;
            case DataType::CAMERA:
                EditCamera(*reinterpret_cast<Renderer::LowLevel::Camera*>(componentData));
                break;
            case DataType::LIGHT:
                EditLight(*reinterpret_cast<Renderer::Light*>(componentData));
                break;
            default:
                break;
            }
        }


        ImGui::TreePop();
    }
}
