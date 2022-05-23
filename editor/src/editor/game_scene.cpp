#include <imgui.h>

#include "renderer/lowlevel/lowrenderer.hpp"

#include "helpers/game_edit.hpp"

#include "engine.hpp"

#include "editor/editor_render.hpp"
#include "editor/game_scene.hpp"


using namespace Editor;
using namespace Game;


void GameScene::OnImGuiRender(Engine& io_engine)
{
    ImGui::Begin("GameScene", nullptr, ImGuiWindowFlags_NoScrollbar);

    bool focusOnGaming = !(io_engine.GetRunMode() & Utils::UpdateFlag_Editing);
    bool canPlayGame = (io_engine.GetRunMode() & Utils::UpdateFlag_Gaming) && ImGui::IsWindowHovered()
                       && (Inputs::IsPressed(EButton::MOUSE_RIGHT) || Inputs::IsPressed(EButton::P));
    if (!focusOnGaming && canPlayGame) // User right click on game window to play
    {
        mouseLockPosition = Inputs::GetMousePosition();
        ImGui::SetWindowFocus(nullptr);
        io_engine.SetRunMode(Utils::UpdateFlag_Gaming);
    }
    else if (focusOnGaming && (Inputs::IsPressed(EButton::MOUSE_RIGHT) || Inputs::IsPressed(EButton::ESCAPE))) // User right click to exit play mode
    {
        Engine::SetCursorPosition(mouseLockPosition);
        io_engine.SetRunMode(Utils::UpdateFlag_Editing | Utils::UpdateFlag_Gaming);
    }

    //ImVec2 windowSize = ImGui::GetWindowSize();
    //io_engine.gameFBO->size = {windowSize.x, windowSize.y};

    EditorRender::gameWindowSize = {ImGui::GetWindowSize().x, ImGui::GetWindowSize().y};

    Vector2 offset = io_engine.gameFBO->offset;
    Vector2 size = io_engine.gameFBO->size;
    ImGui::SetCursorPos({offset.x, offset.y});

    ImGui::Image(reinterpret_cast<ImTextureID>(io_engine.gameFBO->GetColor0()), {size.x, size.y}, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}
