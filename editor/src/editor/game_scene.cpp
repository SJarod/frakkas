#include <imgui.h>

#include "renderer/lowlevel/lowrenderer.hpp"

#include "engine.hpp"

#include "editor/game_scene.hpp"


using namespace Editor;
using namespace Game;


void GameScene::OnImGuiRender(Engine& io_engine)
{
    ImGui::Begin("GameScene");

    bool focusOnGaming = !(io_engine.GetRunMode() & Engine::RunFlag_Editing);
    bool canPlayGame = (io_engine.GetRunMode() & Engine::RunFlag_Gaming) && ImGui::IsWindowHovered()
                       && (Inputs::IsPressed(EButton::MOUSE_RIGHT) || Inputs::IsPressed("pause"));
    if (!focusOnGaming && canPlayGame) // User right click on game window to play
    {
        mouseLockPosition = Inputs::GetMousePosition();
        ImGui::SetWindowFocus(nullptr);
        io_engine.SetRunMode(Engine::RunFlag_Gaming);
    }
    else if (focusOnGaming && Inputs::IsPressed(EButton::MOUSE_RIGHT)) // User right click to exit play mode
    {
        Engine::SetCursorPosition(mouseLockPosition);
        io_engine.SetRunMode(Engine::RunFlag_Editing | Engine::RunFlag_Gaming);
    }

    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImGui::Image(reinterpret_cast<ImTextureID>(io_engine.gameFBO->GetColor0()), windowSize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}