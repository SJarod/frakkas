//
// Created by f.marcellot on 16/03/2022.
//

#pragma once

#include "editor/menu_bar.hpp"
#include "editor/hierarchy.hpp"
#include "editor/console.hpp"
#include "editor/inspector.hpp"
#include "editor/file_browser.hpp"
#include "editor/scene.hpp"
#include "editor/game_scene.hpp"

namespace Renderer::LowLevel
{
    class Framebuffer;
}

namespace Editor
{
    class EditorRender
    {
    public:
        void InitImGui();
        void QuitImGui();
        void UpdateAndRender(Renderer::LowLevel::Framebuffer& io_fbo, Renderer::LowLevel::Camera& io_camera);

    private:
        MenuBar m_menuBar;
        Hierarchy m_hierarchy;
        Console m_console;
        Inspector m_inspector;
        FileBrowser m_fileBrowser;
        Scene m_scene;
        GameScene m_game;

        void UpdateImGui();
    };
}
