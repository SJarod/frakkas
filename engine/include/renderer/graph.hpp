#pragma once

#include <queue>

#include "renderer/lowlevel/camera.hpp"

namespace Game
{
    class Component;
    class CameraComponent;
    class LightComponent;
    class Drawable;

    class EntityManager;
}

namespace Renderer
{
    namespace LowLevel
    {
        class Camera;
        class LowRenderer;
    }

    class Graph
    {
    public:
        Graph(Game::EntityManager* entityManager);
        ~Graph() = default;

        static std::queue<Game::Component*> entityComponentRegistry;

        Renderer::LowLevel::Camera editorCamera;
        Game::CameraComponent* gameCamera = nullptr;

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

    private:
        Game::EntityManager* entityManager;
        std::vector<Game::CameraComponent*> gameCameras;
        std::vector<Game::LightComponent*> lights;
        std::vector<Game::Drawable*> renderEntities;

        std::string_view currentSceneName = "exemple_scene";

        static constexpr char pathToScenes[] = "game/assets/";

        std::string GetFullPath() const;

        void CheckComponentQueue() noexcept;

        /**
         * Searches for the first CameraComponent enabled. Set game camera to nullptr if not found.
         * This function is used when the current game camera is disabled.
         */
        void SetGameCameraAuto() noexcept;

        /**
        * @brief Update uniforms as light or camera's matrices before render entities.
        * @param i_renderer The renderer who possesses the shader to set uniform.
        * @param i_aspectRatio The aspect ratio of the screen to get camera projection matrix.
        */
        void UpdateGlobalUniform(const Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio,
                                 Renderer::LowLevel::Camera& i_camera) const noexcept;

        /**
         * @brief Renders each entity for real.
         * In fact, RenderGame and RenderEditor call this function to process the same way.
         * @param i_renderer the renderer that will draw the entities
         * @param i_aspectRatio the render's aspect ratio
         */
        void Render(Renderer::LowLevel::LowRenderer& i_renderer);
    };
}