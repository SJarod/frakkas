#include <iostream>
#include <imgui.h>

#include "helpers/editor_helpers.hpp"

#include "game/entity.hpp"


using namespace Game;

Entity::~Entity()
{
    for(const std::unique_ptr<Component>& comp : components)
        comp->OnDestroy();
}

void Entity::AddComponent(std::unique_ptr<Component> comp)
{
    comp->owner = this;
    components.emplace_back(std::move(comp));
}

void Entity::Edit()
{
    ImGui::Text("Entity");

    ImGui::Separator();

    Helpers::EditTransform(transform);

    for (const std::unique_ptr<Component>& comp : components)
        comp->Edit();
}