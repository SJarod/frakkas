#include <imgui.h>

#include "game/entity.hpp"
#include "game/entity_manager.hpp"

#include "helpers/game_edit.hpp"

#include "editor/inspector.hpp"


using namespace Editor;


void Inspector::OnImGuiRender(Game::Entity* io_selectedEntity, ImGuizmo::OPERATION& i_guizmoOperation)
{
    ImGui::Begin("Inspector");

    if (io_selectedEntity)
        Helpers::Edit(*io_selectedEntity, i_guizmoOperation);

    ImGui::End();
}
