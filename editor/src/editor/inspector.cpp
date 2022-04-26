#include <imgui.h>

#include "game/entity.hpp"
#include "game/entity_manager.hpp"

#include "helpers/game_edit.hpp"

#include "editor/inspector.hpp"


using namespace Editor;


void Inspector::OnImGuiRender(Game::Entity* io_selectedEntity, Game::EntityManager& io_entityManager, int& i_gizmoType)
{
    ImGui::Begin("Inspector");

    if (io_selectedEntity)
        Helpers::Edit(*io_selectedEntity, io_entityManager.GetEntitiesTransform(), i_gizmoType);

    ImGui::End();
}
