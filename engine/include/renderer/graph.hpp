#pragma once

#include <queue>

#include "renderer/light.hpp"
#include "renderer/lowlevel/camera.hpp"

namespace Game
{
    class Component;
    class CameraComponent;
    class Drawable;
    class Collider;

    class EntityManager;
}

namespace Physic
{
    class PhysicScene;
}

namespace Renderer
{
    namespace LowLevel
    {
        class LowRenderer;
    }

    class Graph
    {
    public:
        Graph(Game::EntityManager* io_entityManager, Physic::PhysicScene* i_physicScene);
        ~Graph() = default;

        bool lightEnabled = true;
        Renderer::Light light;

        Renderer::LowLevel::Camera editorCamera;
        Game::CameraComponent* gameCamera = nullptr;

        /**
         * @brief Inform the graph that a new component had been added.
         * If the component is a drawable or a camera, store it into the graph.
         * @param i_newComponent A pointer of the new component to register.
         */
        static void RegisterComponent(Game::Component* i_newComponent);
        /**
         * @brief Inform the graph that a component will be removed.
         * If the component is a drawable or a camera, remove it from the graph.
         * @param i_oldComponent A pointer of the component to unregister.
         */
        static void UnregisterComponent(Game::Component* i_oldComponent);

        /**
         * @brief Renders each entity, using editor camera
         * @param i_renderer the renderer that will draw the entities
         * @param i_aspectRatio the render's aspect ratio
         */
        void RenderEditor(Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio);
        /**
         * @brief Renders each entity, using game camera
         * @param i_renderer the renderer that will draw the entities
         * @param i_aspectRatio the render's aspect ratio
         */
        void RenderGame(Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio);

        /**
         * Reload the current scene.
         */
        void ReloadScene();
        /**
         * Load a scene from a text file.
         * @param i_sceneName Name of scene, should exists as .kk file.
         */
        void LoadScene(const std::string& i_sceneName);
        /**
         * Save a scene into a text file.
         * @param i_sceneName Name of scene, should exists as .kk file.
         */
        void SaveScene() const;

        static std::string GetSceneFullPath(const std::string& i_sceneName) ;
    private:
        Game::EntityManager* entityManager;
        static Physic::PhysicScene* physicScene;

        static bool updateCamera;
        static std::vector<Game::CameraComponent*> gameCameras;
        static std::vector<Game::Drawable*> renderEntities;

        std::string currentSceneName = "exemple_scene";

        /**
         * Searches for the first CameraComponent enabled. Set game camera to nullptr if not found.
         * This function is used when the current game camera is disabled.
         */
        void SetGameCameraAuto() noexcept;

        /**
        * @brief Update uniforms as light or camera's matrices before render entities.
        * @param i_renderer The renderer who possesses the shader to set uniform.
        * @param i_aspectRatio The aspect ratio of the screen to get camera projection matrix.
         * @param i_camera The camera to project the scene from.
        */
        void UpdateGlobalUniform(const Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio,
                                 Renderer::LowLevel::Camera& i_camera) const noexcept;

        /**
         * @brief Renders each entity for real.
         * In fact, RenderGame and RenderEditor call this function to process the same way.
         * @param i_renderer the renderer that will draw the entities
         */
        void Render(Renderer::LowLevel::LowRenderer& i_renderer);

        /**
         * @brief Render all enabled collider.
         * @param i_renderer The renderer that will draw the colliders.
         */
        void RenderColliders(Renderer::LowLevel::LowRenderer& i_renderer);
    };
}
