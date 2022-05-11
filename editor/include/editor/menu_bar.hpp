#pragma once

class Engine;

namespace Game
{
    class Entity;
    class EntityManager;
}

namespace Renderer
{
    class Graph;
    class Light;
}

class Engine;

namespace Editor
{
    class MenuBar
    {
    public:
        MenuBar() = default;
        ~MenuBar() = default;

        /**
        * @brief Draw the top panel with useful menus.
        * @param io_engine The engine to interact with the entire system.
        * @param o_loadScene An output boolean to notify editor render when the scene is reload.
        * @param io_selectedEntity The entity selected in hierarchy.
        */
        void OnImGuiRender(Engine& io_engine, bool& o_loadScene, Game::Entity* io_selectedEntity);
        void FrakkasColors();

    private:
        // Scene management (Save, Load, Reload, New)
        void FileField(Renderer::Graph& io_graph, bool& o_loadScene);
        static bool CreateScenePopup(Renderer::Graph& io_graph);
        static bool OpenScenePopup(Renderer::Graph& io_graph);

        // Entity editing (copy, past)
        void EditField(Game::EntityManager& io_entityManager, Game::Entity* io_selectedEntity);

        // Editor style option
        void OptionsField();

        // Game interaction (Add component, Play, Pause, Stop) 
        void GameField(Engine& io_engine, bool& o_loadScene);
        static void CreateComponentPopup();

        /**
         * @brief Edit lighting
         * @param io_engine The class that posseses the graph, the light, and the depth map.
         */
        void LightingField(Engine& io_engine);
    };
}
