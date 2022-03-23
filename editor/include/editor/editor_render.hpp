//
// Created by f.marcellot on 16/03/2022.
//

#pragma once

#include "editor/menu_bar.hpp"
#include "editor/hierarchy.hpp"
#include "editor/console.hpp"
#include "editor/inspector.hpp"
#include "editor/resource_viewer.hpp"
#include "editor/scene.hpp"
#include "editor/game.hpp"


namespace Editor
{
    class EditorRender
    {
    public:
        void InitImGui();
        void QuitImGui();
        void UpdateAndRender(bool ShowDemoWindow);

    private:
        MenuBar m_menuBar;
        Hierarchy m_hierarchy;
        Console m_console;
        Inspector m_inspector;
        ResourceViewer m_resourceViewer;
        Scene m_scene;
        Game m_game;

        void UpdateImGui();
    };
}
