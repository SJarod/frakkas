#include <cassert>
#include <fstream>

#include "resources/serializer.hpp"

#include "debug/log.hpp"

#include "game/entity.hpp"
#include "game/entity_manager.hpp"

using namespace Game;

void EntityManager::Update()
{
    for (const auto& entity : entities)
    {
        for (const std::unique_ptr<Component>& comp: entity->components)
        {
            if (comp->enabled)
                comp->Update();
        }
    }
}

void EntityManager::AddEntity(std::unique_ptr<Entity> i_entity)
{
    for (const std::unique_ptr<Component>& comp : i_entity->components)
    {
        comp->Start();
    }
    // Store entity's transform in the map
    transforms[i_entity->name.c_str()] = &i_entity->transform;
}

void EntityManager::RemoveEntityAt(int i_index)
{
    if (i_index >= entities.size())
    {
        Log::Warning("Can't remove entity with index ", i_index,". Index out of bound.");
        return;
    }

    Entity* entity = entities[i_index].get();

    // Clear transform childs
    entity->transform.ClearChilds();
    if (entity->transform.parent)
        entity->transform.parent.get()->RemoveChild(&entity->transform);

    // Remove entity's components
    for (int i = entity->components.size()-1; i >= 0; i--)
        entity->RemoveComponentAt(i);

    // Remove entity's transform
    transforms.erase(entities[i_index]->name.c_str());
    // Remove entity
    entities.erase(entities.begin() + i_index);
}

Entity* EntityManager::CreateEntity(const std::string_view& i_name)
{
    entities.emplace_back(std::make_unique<Entity>(i_name));

    Entity* entity = entities.back().get();
    // Store entity's transform in the map
    transforms[entity->name.c_str()] = &entity->transform;
    return entity;
}

void EntityManager::CreateEntities(std::ifstream& i_file)
{
    // Reset
    entities.clear();
    // Load
    std::string attribute;
    i_file.ignore(); // skip empty lines
    while (!i_file.eof())
    {
        Resources::Serializer::GetAttribute(i_file, attribute); // skip '>entity'
        if (attribute == "entity")
        {
            Entity* entity = CreateEntity("nameholders");
            Resources::Serializer::Read(i_file, *entity);
        }
        i_file.ignore(); // skip empty lines
    }
}

const std::vector<std::unique_ptr<Entity>> & EntityManager::GetEntities() const
{
    return entities;
}

const std::unordered_map<const char*, Transform*>& EntityManager::GetEntitiesTransform() const
{
    return transforms;
}
