#include <iostream>

#include "game/entity.hpp"


using namespace Game;

Entity::~Entity()
{
    for(const std::unique_ptr<Component>& comp : components)
        comp->OnDestroy();
}

Component* Entity::AddComponent(std::unique_ptr<Component> comp)
{
    comp->owner = this;
    components.emplace_back(std::move(comp));
    return components.back().get();
}
