#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <string_view>

#include <unordered_map>

#include "entity.hpp"

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
         * @summary Remove an entity from the entities vector according to its id.
         * @param i_id  The ID of the entity to remove.
         */
        void RemoveEntityAt(const EntityIdentifier& i_id);

        /**
         * @summary Creates an unique-pointer-empty-entity and emplace it in entities array
         * @return the created entity, you can add component to it.
         */
        Entity* CreateEntity(const std::string_view& i_name = "");

        /**
         * @summary Creates an unique-pointer-empty-entity and emplace it in entities array
         * @param i_id Force a specific ID. If the ID is not accessible, it will generates another one.
         * @return the created entity, you can add component to it.
         */
        Entity* CreateEntity(const uint64_t& i_id, const std::string_view& i_name = "");

        /**
         * @return The entity with the input ID, thinks about checking if return value is not nullptr.
         */
        Entity* GetEntityAt(const EntityIdentifier& i_id);

        /**
         * @brief read all entities from the file, then create and store them in this manager
         * @param i_file the opened input file
         */
        void CreateEntities(std::ifstream& i_file);

        /**
         * @brief Reset all entities array to 0.
         */
        void ClearEntities();

        /**
         * @brief Add a parent ot an entity, and update root entities map.
         * @param io_child The child to set the parent
         * @param io_parent The parent, will be informed about its new child
         */
        void SetEntityParent(Entity& io_child, Entity& io_parent);
        /**
         * @brief Set the parent to nullptr for io_child.
         * If parent is not nullptr, we remove io_child from its children.
         * @param io_child The entity to remove parent.
         */
        void UnsetEntityParent(Entity& io_child);

        const std::vector<std::unique_ptr<Entity>>& GetEntities() const;
        const std::unordered_map<EntityIdentifier, Entity*>& GetRootEntities() const;

    private:
        std::vector<std::unique_ptr<Entity>> entities;
        std::unordered_map<EntityIdentifier, Entity*> rootEntities;

        static EntityIdentifier maxID;

        /**
         * @brief Remove entity from all arrays, and forget about its existence.
         * @param io_entity The entity to forget.
         */
        void ForgetEntity(Entity& io_entity);

        /**
         * @brief Find the entity position in entities array according to its index.
         * @return The index in the entities array. Return -1 if index out of bound.
         */
        int FindEntityIndex(const EntityIdentifier& i_id);

        /**
         * @brief Read the entity's data from an input file, and set the parent if exists.
         * @param i_file The opened input file.
         * @param i_parent The parent to set if it is not nullptr.
         */
        void LoadEntity(std::ifstream& i_file, Entity* i_parent);
    };
}
