#include <iostream>

#include "renderer/graph.hpp"

#include "physic/physic_scene.hpp"

#include "game/lowcomponent/collider/collider.hpp"
#include "game/entity.hpp"


using namespace Game;

Entity::Entity(const EntityIdentifier& i_id, const std::string_view& i_name)
    :id(i_id)
{
    if (i_name.empty())
        name = "NewEntity_" + std::to_string(id);
    else
        name = i_name;
}

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

void Entity::NotifyCollision(Physic::ECollisionEvent i_collisionType, Collider* i_ownerCollider, Collider* i_otherCollider)
{
    switch (i_collisionType)
    {
    case Physic::ECollisionEvent::COLLISION_ENTER:
        for (const std::unique_ptr<Component>& comp : components)
            comp->OnCollisionEnter(i_ownerCollider, i_otherCollider);
        break;
    case Physic::ECollisionEvent::COLLISION_STAY:
        for (const std::unique_ptr<Component>& comp : components)
            comp->OnCollisionStay(i_ownerCollider, i_otherCollider);
        break;
    case Physic::ECollisionEvent::COLLISION_EXIT:
        for (const std::unique_ptr<Component>& comp : components)
            comp->OnCollisionExit(i_ownerCollider, i_otherCollider);
        break;
    case Physic::ECollisionEvent::TRIGGER_ENTER:
        for (const std::unique_ptr<Component>& comp : components)
            comp->OnTriggerEnter(i_ownerCollider, i_otherCollider);
        break;
    case Physic::ECollisionEvent::TRIGGER_STAY:
        for (const std::unique_ptr<Component>& comp : components)
            comp->OnTriggerStay(i_ownerCollider, i_otherCollider);
        break;
    case Physic::ECollisionEvent::TRIGGER_EXIT:
        for (const std::unique_ptr<Component>& comp : components)
            comp->OnTriggerExit(i_ownerCollider, i_otherCollider);
        break;
    default:
        break;
    }
}

void Entity::RegisterIntoGraph(Component* i_newComponent) const
{
    if (id != 0)
        Renderer::Graph::RegisterComponent(i_newComponent);
}

void Entity::UnregisterIntoGraph(Component* i_oldComponent) const
{
    if (id != 0)
        Renderer::Graph::UnregisterComponent(i_oldComponent);
}
