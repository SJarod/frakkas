#pragma once

#include <vector>
#include <memory>

#include "resources/serializer.hpp"


namespace Renderer::LowLevel
{
    class LowRenderer;
    class Camera;
}

namespace Game
{
    class Entity;

    class EntityManager
    {
    public:
        EntityManager() = default;
        ~EntityManager() = default;

        Renderer::LowLevel::Camera* camera = nullptr;

        /**
         * Update every entities
         */
        void Update();

        /**
         * @brief Render each entity
         * @param i_renderer the renderer that will draw the entities
         * @param i_camera the camera to render, in other words the source view to look at the world
         * @param i_aspectRatio the render's aspect ratio
         */
        void Render(Renderer::LowLevel::LowRenderer &i_renderer, const float i_aspectRatio);

        /**
         * @summary move an entity pointer into the entity manager's array,
         * the function also call the Start() method of the input entity
         * @param i_entity the fully constructed entity to add, it is an unique pointer so use std::move()
         */
        void AddEntity(std::unique_ptr<Entity> i_entity);

        [[nodiscard]] const std::list<std::unique_ptr<Entity>> & GetEntities() const;

        /**
         * Create entity from a scene input file.
         * @param i_file the opened input file.
         */
        void Read(std::ifstream& i_file, const Resources::Serializer& i_serializer) {};

        /**
         * Write all the entities in a scene output file.
         * @param o_file the opened output file.
         */
        void Write(std::ofstream& o_file, const Resources::Serializer& i_serializer) {};

    private:

        std::list<std::unique_ptr<Entity>> entities;
    };
}