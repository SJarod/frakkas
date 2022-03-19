//
// Created by flori on 3/17/2022.
//

#pragma once

#include <vector>
#include <memory>

class Entity;

namespace Engine
{
    class EntityManager
    {
    public:
        ~EntityManager() = default;

        /**
         * The fundamental function of singleton class. Construct the instance when called for the first time.
         * @return The unique instance of EntityManager.
         */
        static EntityManager& GetInstance();
        static void Init();

        /**
         * Update every entities
         */
        static void UpdateAndRender();

        /**
         * @summary move an entity pointer into the entity manager's array,
         * the function also call the Start() method of the input entity
         * @param i_entity the constructed entity to add, it is an unique pointer so use std::move()
         */
        static void AddEntity(std::unique_ptr<Entity> i_entity);

    private:
        EntityManager() = default;

        static std::shared_ptr<EntityManager> instance;

        std::vector<std::unique_ptr<Entity>> entities;

    };
}
