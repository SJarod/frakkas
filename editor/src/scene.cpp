//
// Created by m.mehalin on 22/03/2022.
//

#include <imgui.h>

#include "editor/scene.hpp"

using namespace Editor;


Vector2 Scene::OnImGuiRender(ImTextureID i_tex)
{
    ImGui::Begin("Scene");

	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	ImGui::Image(i_tex, windowSize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

	return { windowSize.x, windowSize.y };
}
