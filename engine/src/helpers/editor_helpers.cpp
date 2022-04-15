#include <imgui.h>

#include "game/transform.hpp"
#include "game/component.hpp"
#include "game/light_component.hpp"

#include "helpers/editor_helpers.hpp"


void Helpers::EditTransform(Game::Transform &io_transform)
{
    Game::Transform& trs = io_transform;
    Game::ScaleLockParams& scParams = trs.GetScaleLockParameters();

    ImGui::Text("Transform");

    ImGui::Spacing();

    Vector3 pos = trs.position, rot = Maths::ToDegrees(trs.rotation), sc = trs.scale;

// Position edit
    ImGui::DragFloat3("Position", pos.element, 0.1f);

// Rotation edit
    ImGui::DragFloat3("Rotation", rot.element, 1.f);
    for (float & coord : rot.element)
        coord = Maths::Abs(coord) >= 360.f ? 0.f : coord;

#pragma region Scale edit
    if(scParams.isLock && scParams.origScale == Vector3::zero)
        scParams.ratio = 0.f;

    if(ImGui::DragFloat3("Scale", sc.element, 0.1f))
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

void Helpers::EditLight(Renderer::Light &io_light)
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

bool Helpers::ComponentBeginEdit(Game::Component *comp)
{
    if(ImGui::TreeNodeEx(comp->GetID().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        bool checkEnabled = comp->enabled;
        ImGui::Checkbox("Enabled", &checkEnabled);
        comp->enabled = checkEnabled;
        ImGui::Spacing();
        return true;
    }
    return false;
}

void Helpers::ComponentEndEdit()
{
    ImGui::TreePop();
}
