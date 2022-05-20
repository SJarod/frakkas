#pragma once

#include "editor/menu_bar.hpp"
#include "editor/hierarchy.hpp"
#include "editor/console.hpp"
#include "editor/inspector.hpp"
#include "editor/file_browser.hpp"
#include "editor/scene.hpp"
#include "editor/game_scene.hpp"
#include "editor/debugger.hpp"
#include "editor/resources_viewer.hpp"
#include "engine.hpp"


namespace Renderer::LowLevel
{
    class Framebuffer;
}

namespace Game
{
    class EntityManager;
}

class Engine;

namespace Editor
{
    class EditorRender
    {
    public:
        static bool editingDrag;
        static bool editingText;
        static Vector2 mouseLockPosition;
        static Vector2 gameWindowSize;

        void InitImGui(Engine& io_engine);
        void QuitImGui();
        /**
         * @brief Create all the editor interface and allow user to edit its game.
         * @param io_engine The engine to edit.
         */
        void UpdateAndRender(Engine& io_engine);

    private:
        ImGuizmo::OPERATION guizmoOperation = ImGuizmo::OPERATION::BOUNDS;

        MenuBar m_menuBar;
        Hierarchy m_hierarchy;
        Console m_console;
        Inspector m_inspector;
        FileBrowser m_fileBrowser;
        Scene m_scene;
        GameScene m_game;
        Debugger m_debugger;
        ResourcesViewer m_resources;

        void UpdateImGui();
    };
}
