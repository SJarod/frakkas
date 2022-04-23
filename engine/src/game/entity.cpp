#include <iostream>

#include "renderer/graph.hpp"

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
    RegisterIntoGraph(components.back().get());
    return components.back().get();
}

void Entity::RegisterIntoGraph(Component* i_newComponent)
{
    Renderer::Graph::entityComponentRegistry.emplace(i_newComponent);
}
