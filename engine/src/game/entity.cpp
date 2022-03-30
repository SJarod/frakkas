//
// Created by flori on 3/17/2022.
//

#include <iostream>
#include "imgui.h"

#include "resources/serializer.hpp"

#include "game/entity.hpp"

using namespace Game;

void Entity::AddComponent(const std::shared_ptr<Component>& comp) {
    comp->owner = this;
    components.push_back(comp);
}



void Entity::Edit() {

    ImGui::Text("Entity");

    ImGui::Separator();

    transform.Edit();

    for (const std::shared_ptr<Component>& comp : components)
        comp->Edit();
}

