#include <imgui.h>

#include "game/entity.hpp"

#include "editor/inspector.hpp"


using namespace Editor;


void Inspector::OnImGuiRender(Game::Entity* io_selectedEntity)
{
    ImGui::Begin("Inspector");

    if (io_selectedEntity)
        io_selectedEntity->Edit();

    ImGui::End();
}