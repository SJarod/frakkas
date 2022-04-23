#include <cassert>
#include <fstream>

#include "resources/mesh.hpp"
#include "resources/serializer.hpp"
#include "resources/resources_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"
#include "game/entity.hpp"
#include "game/drawable.hpp"
#include "game/camera_component.hpp"
#include "game/light_component.hpp"

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
}

Entity* EntityManager::CreateEntity(const std::string_view& i_name)
{
    entities.emplace_back(std::make_unique<Entity>(i_name));
    return entities.back().get();
}

const std::list<std::unique_ptr<Entity>> & EntityManager::GetEntities() const
{
    return entities;
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
        Serializer::GetAttribute(i_file, attribute); // skip '>entity'
        if (attribute == "entity")
        {
            Entity* entity = CreateEntity("nameholders");
            Serializer::Read(i_file, *entity);
        }
        i_file.ignore(); // skip empty lines
    }
}
