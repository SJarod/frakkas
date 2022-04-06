#pragma once

#include <vector>
#include <memory>

#include "renderer/lowlevel/camera.hpp"

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
    class Entity;
    class CameraComponent;

    class EntityManager
    {
    public:
        EntityManager();
        ~EntityManager() = default;

        Renderer::LowLevel::Camera editorCamera;
        CameraComponent* gameCamera = nullptr;

        /**
         * Update every entities
         */
        void Update();

        /**
         * @brief Renders each entity
         * @param i_renderer the renderer that will draw the entities
         * @param i_aspectRatio the render's aspect ratio
         */
        void Render(Renderer::LowLevel::LowRenderer& i_renderer, const float i_aspectRatio);

        /**
         * @brief The game udpate and render called every frames
         * @param i_renderer the renderer that will draw the entities
         * @param i_aspectRatio the render's aspect ratio
         */
        void UpdateAndRender(Renderer::LowLevel::LowRenderer &i_renderer, const float i_aspectRatio);

        /**
         * @summary Moves an entity pointer into the entity manager's array,
         * the function also call the Start() method of the input entity
         * @param i_entity the fully constructed entity to add, it is an unique pointer so use std::move()
         */
        void AddEntity(std::unique_ptr<Entity> i_entity);

        /**
         * @summary Creates an unique-pointer-empty-entity and emplace it in entities array
         * @return the created entity, you can add component to it.
         */
        Entity* CreateEntity();

        const std::list<std::unique_ptr<Entity>>& GetEntities() const;

        /**
         * Creates entity from a scene input file.
         * @param i_file the opened input file.
         */
        void Read(std::ifstream& i_file, const Resources::Serializer& i_serializer) {};

        /**
         * Writes all the entities in a scene output file.
         * @param o_file the opened output file.
         */
        void Write(std::ofstream& o_file, const Resources::Serializer& i_serializer) {};

    private:

        std::list<std::unique_ptr<Entity>> entities;

        /**
         * Searches for the first entity that owns a CameraComponent enabled. Set game camera to nullptr if not found.
         * This function is used when the current game camera is disabled.
         */
        void FindGameCamera() noexcept;
    };
}
