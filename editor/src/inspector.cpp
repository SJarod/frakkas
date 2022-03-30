//
// Created by m.mehalin on 22/03/2022.
//

#include <imgui.h>

#include "game/entity.hpp"

#include "editor/inspector.hpp"

using namespace Editor;


void Inspector::OnImGuiRender(Game::Entity *selectedEntity)
{
    ImGui::Begin("Inspector");

    if (selectedEntity)
        selectedEntity->Edit();

    ImGui::End();
}