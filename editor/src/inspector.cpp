//
// Created by m.mehalin on 22/03/2022.
//

#include <imgui.h>

#include "game/engine_entity.hpp"

#include "editor/inspector.hpp"

using namespace Editor;


void Inspector::OnImGuiRender(Game::EngineEntity *selectedEntity)
{
    ImGui::Begin("Inspector");

    if (selectedEntity)
        selectedEntity->Edit();

    ImGui::End();
}