#include <iostream>
#include <imgui.h>

#include "helpers/editor_helpers.hpp"
#include "log.hpp"

#include "resources/serializer.hpp"

#include "game/entity.hpp"

using namespace Game;


Entity::~Entity()
{
    for(const std::shared_ptr<Component>& comp : components)
        comp->OnDestroy();
}

void Entity::AddComponent(const std::shared_ptr<Component>& comp) {
    comp->owner = this;
    components.push_back(comp);
}



void Entity::Edit() {

    ImGui::Text("Entity");

    ImGui::Separator();

    Helpers::EditTransform(transform);

    for (const std::shared_ptr<Component>& comp : components)
        comp->Edit();
}

