//
// Created by flori on 3/17/2022.
//

#pragma once

#include <vector>
#include <memory>

namespace Renderer::LowLevel
{
    class LowRenderer;
    class Camera;
}

namespace Engine
{
    class EngineEntity;

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
         * @brief Render each entity
         * @param i_renderer the renderer that will draw the entities
         * @param i_camera the camera to render, in other words the source view to look at the world
         */
        void Render(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::LowLevel::Camera& i_camera);

        /**
         * @summary move an entity pointer into the entity manager's array,
         * the function also call the Start() method of the input entity
         * @param i_entity the fully constructed entity to add, it is an unique pointer so use std::move()
         */
        void AddEntity(std::unique_ptr<EngineEntity> i_entity);

    private:

        std::vector<std::unique_ptr<EngineEntity>> entities;
    };
}
