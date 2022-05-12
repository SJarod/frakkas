#pragma once

#include "game/entity.hpp"

namespace Game
{
    class Transform;

    /**
     * @brief Class to store entities and give useful utils to Create and Get entity
     */
    class EntityContainer
    {
    public:
        std::vector<std::unique_ptr<Entity>> entities;

        /**
         * @summary Creates an unique-pointer-empty-entity and emplace it in entities array
         * @return the created entity, you can add component to it.
         */
        Entity* CreateEntity(const std::string_view& i_name = "");

        /**
         * @return The entity with the input ID, thinks about checking if return value is not nullptr.
         */
        Entity* FindEntityWithID(const EntityIdentifier& i_id);

        /**
         * @tparam TComponent Class name of the component
         * @return The first entity with the input component, nullptr if nothing found.
         */
        template<typename TComponent>
        Entity* FindEntityWithComponent();

    private:
        static EntityIdentifier maxID;
    };
}

template<typename TComponent>
Game::Entity* Game::EntityContainer::FindEntityWithComponent()
{
    for (const std::unique_ptr<Entity>& entity : entities)
    {
        if (entity->GetComponent<TComponent>())
            return entity.get();
    }

    return nullptr;
}


