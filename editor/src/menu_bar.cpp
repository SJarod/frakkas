//
// Created by m.mehalin on 16/03/2022.
//

#include <imgui.h>

#include "editor/menu_bar.hpp"

using namespace Editor;


void MenuBar::OnImGuiRender()
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit"))
    {
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}