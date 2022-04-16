#include <imgui.h>

#include "game/entity.hpp"
#include "helpers/game_edit.hpp"

#include "editor/inspector.hpp"


using namespace Editor;


void Inspector::OnImGuiRender(Game::Entity* io_selectedEntity)
{
    ImGui::Begin("Inspector");

    if (io_selectedEntity)
        Helpers::EditEntity(*io_selectedEntity);

    ImGui::End();
}