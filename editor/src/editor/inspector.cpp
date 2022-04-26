#include <imgui.h>

#include "game/entity.hpp"
#include "helpers/game_edit.hpp"

#include "editor/inspector.hpp"


using namespace Editor;


void Inspector::OnImGuiRender(Game::Entity* io_selectedEntity, int& i_gizmoType)
{
    ImGui::Begin("Inspector");

    if (io_selectedEntity)
        Helpers::Edit(*io_selectedEntity, i_gizmoType);

    ImGui::End();
}
