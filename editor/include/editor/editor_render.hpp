#pragma once

#include "editor/menu_bar.hpp"
#include "editor/hierarchy.hpp"
#include "editor/console.hpp"
#include "editor/inspector.hpp"
#include "editor/file_browser.hpp"
#include "editor/scene.hpp"
#include "editor/game_scene.hpp"
#include "editor/debugger.hpp"


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
        void InitImGui(Engine& io_engine);
        void QuitImGui();
        void UpdateAndRender(Engine& io_engine);

        static bool isEditingDrag;
        static Vector2 mouseLockPosition;

    private:
        MenuBar m_menuBar;
        Hierarchy m_hierarchy;
        Console m_console;
        Inspector m_inspector;
        FileBrowser m_fileBrowser;
        Scene m_scene;
        GameScene m_game;
        Debugger m_debugger;

        void UpdateImGui();
    };
}
