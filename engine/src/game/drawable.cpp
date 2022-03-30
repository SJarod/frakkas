#include <imgui.h>

#include "game/entity.hpp"

#include "game/drawable.hpp"

using namespace Game;

Drawable::Drawable() : Component("drawable") {}

void Drawable::Edit() {
    if(ImGui::TreeNodeEx("Drawable", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Spacing();
        model.Edit();
        ImGui::TreePop();
    }
}

void Drawable::SetOwner(Entity *owner) {
    model.transform.parent = &owner->transform;
    Component::SetOwner(owner);
};