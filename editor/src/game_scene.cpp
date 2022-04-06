#include <imgui.h>

#include "editor/game_scene.hpp"


using namespace Editor;


void GameScene::OnImGuiRender(ImTextureID i_tex)
{
    ImGui::Begin("GameScene");

    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImGui::Image(i_tex, windowSize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}