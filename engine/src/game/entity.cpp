#include <iostream>

#include "renderer/graph.hpp"

#include "game/entity.hpp"


using namespace Game;

Entity::Entity(const EntityIdentifier& i_id, const std::string_view& i_name)
    :id(i_id)
{
    if (i_name.empty())
        name = "NewEntity_" + std::to_string(id);
    else
        name = i_name;
};

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

void Entity::RemoveComponentAt(int index)
{
    // Call OnDestroy event
    components[index]->OnDestroy();
    // Unregister
    UnregisterIntoGraph(components[index].get());
    components.erase(components.begin() + index);
}

void Entity::RegisterIntoGraph(Component* i_newComponent)
{
    Renderer::Graph::RegisterComponent(i_newComponent);
}

void Entity::UnregisterIntoGraph(Component* i_oldComponent)
{
    Renderer::Graph::UnregisterComponent(i_oldComponent);
}
