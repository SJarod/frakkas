#pragma once

#include <memory>
#include <vector>
#include <string>

#include <unordered_map>


namespace Renderer::LowLevel
{
    class LowRenderer;
}

namespace Resources
{
    class Serializer;
}
namespace Game
{
    class Transform;
    class Entity;

    class EntityManager
    {
    public:
        EntityManager() = default;
        ~EntityManager() = default;

        /**
         * Update every entities
         */
        void Update();

        /**
         * @summary Moves an entity pointer into the entity manager's array,
         * the function also call the Start() method of the input entity
         * @param i_entity the fully constructed entity to add, it is an unique pointer so use std::move()
         */
        void AddEntity(std::unique_ptr<Entity> i_entity);

        /**
         * @summary Remove an entity from the entities array according to index.
         * @param i_index The index of the entity to remove.
         */
        void RemoveEntityAt(int i_index);

        /**
         * @summary Creates an unique-pointer-empty-entity and emplace it in entities array
         * @return the created entity, you can add component to it.
         */
        Entity* CreateEntity(const std::string_view& i_name);

        /**
         * @brief read all entities from the file, then create and store them in this manager
         * @param i_file the opened input file
         */
        void CreateEntities(std::ifstream& i_file);

        const std::vector<std::unique_ptr<Entity>>& GetEntities() const;
        const std::unordered_map<const char*, Transform*>& GetEntitiesTransform() const;

    private:
        std::vector<std::unique_ptr<Entity>> entities;
        std::unordered_map<const char*, Transform*> transforms;
    };
}
