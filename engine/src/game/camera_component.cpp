#include <imgui.h>

#include "helpers/editor_helpers.hpp"

#include "game/camera_component.hpp"

using namespace Game;

CameraComponent::CameraComponent() :Component("camera"){}

void CameraComponent::Edit() {
    if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen))
    {
        Helpers::EditTransform(camera.transform);
        ImGui::Spacing();

        float fovy = Maths::ToDegrees(camera.targetFovY);
        ImGui::SliderFloat("Field of view Y", &fovy, 0.f, 180.f);
        camera.targetFovY = Maths::ToRadians(fovy);

        ImGui::DragFloat("Near", &camera.near, 0.01f, 0.001f);
        ImGui::DragFloat("Far", &camera.far, 10.f, 100.f, 5000.f);

        ImGui::TreePop();
    }
}


