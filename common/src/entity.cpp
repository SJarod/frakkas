//
// Created by flori on 3/17/2022.
//

#include <iostream>

#include "entity.hpp"

Entity::~Entity() {
    for(const std::shared_ptr<Component>& comp : components)
        comp->OnDestroy();
}

void Entity::Start() {

    std::cout << "Entity started" << std::endl;

    for (const std::shared_ptr<Component>& comp : components)
        comp->Start();
}

void Entity::Update() {

    std::cout << "Entity updated" << std::endl;

    for(const std::shared_ptr<Component>& comp : components)
    {
        if (comp->IsEnabled())
            comp->Update();
    }
}

void Entity::AddComponent(const std::shared_ptr<Component>& comp) {
    components.push_back(comp);
}

