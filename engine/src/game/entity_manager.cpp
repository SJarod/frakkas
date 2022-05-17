#include <fstream>

#include "resources/serializer.hpp"

#include "debug/log.hpp"

#include "game/transform.hpp"
#include "game/entity_manager.hpp"
#include "game/entity.hpp"


using namespace Game;

EntityIdentifier EntityContainer::maxID = 1; // Begin at 1, because '0' is index none

Entity* EntityContainer::CreateEntity(const std::string_view& i_name)
{
    entities.emplace_back(std::make_unique<Entity>(maxID++,  i_name));

    Entity* entity = entities.back().get();
    Log::Info("Create new entity : '", entity->name, "'");
    entity->entityStore = this;
    return entity;
}

Entity* EntityContainer::FindEntityWithID(const EntityIdentifier& i_id)
{
    auto findPredicate = [&i_id](const std::unique_ptr<Entity>& entity)
    {
        return i_id == entity->GetID();
    };
    const auto& it = std::find_if(entities.begin(), entities.end(), findPredicate);

    if (it != entities.end())
        return it->get();

    return nullptr;
}

void EntityManager::Start()
{
    for (const auto& entity : entityStore.entities)
        for (const std::unique_ptr<Component>& comp: entity->components)
            comp->OnStart();
}

void EntityManager::Update()
{
    for (int i = entityStore.entities.size()-1; i >= 0; i--)
    {
        Entity* entity = entityStore.entities[i].get();
        for (const std::unique_ptr<Component>& comp: entity->components)
        {
            if (comp->enabled)
                comp->OnUpdate();
        }
        if (entity->destroy)
            RemoveEntityAt(entity->GetID());
    }
}

void Game::EntityManager::RemoveEntityAt(const EntityIdentifier& i_id)
{
    int index = FindEntityIndex(i_id);

    if (index != -1)
    {
        Log::Info("Remove entity '", entityStore.entities[index]->name, "'");

        ForgetEntity(*entityStore.entities[index]);
    }
}

void Game::EntityManager::ForgetEntity(Entity& io_entity)
{
    // If entity has childs, remove them then recall the remove function with no childs
    if (!io_entity.childs.empty())
    {
        // Remove entity childs
        while (!io_entity.childs.empty())
            RemoveEntityAt(io_entity.childs.front()->GetID());

        io_entity.childs.clear();

        RemoveEntityAt(io_entity.GetID());
        return;
    }

    // If entity has parent, remove child into the parent list
    UnsetEntityParent(io_entity);

    // Clear transform childs
    io_entity.transform.ClearChilds();
    if (io_entity.transform.parent)
        io_entity.transform.parent.get()->RemoveChild(&io_entity.transform);

    // Remove entity's components
    for (int i = io_entity.components.size() - 1; i >= 0; i--)
        io_entity.RemoveComponentAt(i);

    // Remove entity from root map
    if (rootEntities.find(io_entity.GetID()) != rootEntities.end())
        rootEntities.erase(io_entity.GetID());

    entityStore.entities.erase(entityStore.entities.begin() + FindEntityIndex(io_entity.GetID()));
}

Entity* EntityManager::CreateEntity(const std::string_view& i_name)
{
    Entity* entity = entityStore.CreateEntity(i_name);
    // Set entity as root by default
    rootEntities[entity->GetID()] = entity;
    return entity;
}

Entity* Game::EntityManager::FindEntityWithID(const EntityIdentifier& i_id)
{
    return entityStore.FindEntityWithID(i_id);
}

void EntityManager::LoadEntities(std::ifstream& i_file)
{
    // Reset
    ClearEntities();
    // Load
    i_file.ignore(); // skip empty lines
    while (!i_file.eof())
        Resources::Serializer::CreateAndReadEntity(i_file, *this, nullptr);
}

void Game::EntityManager::ClearEntities()
{
    entityStore.entities.clear();
    rootEntities.clear();
}

void Game::EntityManager::SetEntityParent(Entity& io_child, Entity& io_parent)
{
    io_child.transform.parent = &io_parent.transform;

    // Check if parent is accepted
    if (io_child.transform.parent == &io_parent.transform)
    {
        io_child.parent = &io_parent;
        io_parent.childs.emplace_back(&io_child);
        rootEntities.erase(io_child.GetID()); // Remove child from root if needed
    }
}

void Game::EntityManager::UnsetEntityParent(Entity& io_child)
{
    // Check that parent exists
    if (!io_child.parent)
        return;

    Entity& parent = *io_child.parent;
    // Check that parent is indeed a parent
    if (io_child.transform.parent == &parent.transform)
    {
        io_child.transform.parent.get()->RemoveChild(&io_child.transform);
        io_child.transform.parent = nullptr;
        io_child.parent = nullptr;
        rootEntities[io_child.GetID()] = &io_child;

        parent.childs.remove_if([&io_child](Entity* parent) { return io_child.GetID() == parent->GetID(); }); // remove if ID is the same
    }

}

const std::vector<std::unique_ptr<Entity>> & EntityManager::GetEntities() const
{
    return entityStore.entities;
}

const std::unordered_map<EntityIdentifier, Entity*>& Game::EntityManager::GetRootEntities() const
{
    return rootEntities;
}

int Game::EntityManager::FindEntityIndex(const EntityIdentifier& i_id)
{
    auto findPredicate = [&i_id](const std::unique_ptr<Entity>& entity)
    {
        return i_id == entity->GetID();
    };

    int index = std::find_if(entityStore.entities.begin(), entityStore.entities.end(), findPredicate) - entityStore.entities.begin();

    if (index >= entityStore.entities.size())
    {
        Log::Warning("Try accessing an entity with index ", index, ". Index out of bound.");
        return -1;
    }

    return index;
}
