#include <imgui.h>

#include "renderer/lowlevel/lowrenderer.hpp"

#include "engine.hpp"

#include "editor/game_scene.hpp"


using namespace Editor;
using namespace Game;


void GameScene::OnImGuiRender(const Renderer::LowLevel::Framebuffer& i_fbo, bool i_gaming)
{
    ImGui::Begin("GameScene");

    bool canPlayGame = i_gaming && ImGui::IsWindowHovered()
                       && (Inputs::IsPressed(EButton::MOUSE_RIGHT) || Inputs::IsPressed("pause"));
    if (!focusOnGaming && canPlayGame) // User right click on game window to play
    {
        mouseLockPosition = Inputs::GetMousePosition();
        Engine::SetCursorGameMode(true);
        ImGui::SetWindowFocus(nullptr);
        focusOnGaming = true;
    }
    else if (focusOnGaming && Inputs::IsPressed(EButton::MOUSE_RIGHT)) // User right click to exit play mode
    {
        Engine::SetCursorGameMode(false);
        Engine::SetCursorPosition(mouseLockPosition);
        focusOnGaming = false;
    }

    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImGui::Image(reinterpret_cast<ImTextureID>(i_fbo.GetColor0()), windowSize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}