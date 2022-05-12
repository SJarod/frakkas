#include "editor/editor_render.hpp"

#include "game/world.hpp"

#include "engine.hpp"

#undef main
int main()
{
    Engine engine;
    Game::World world(engine);

	Editor::EditorRender editorRender{};
    editorRender.InitImGui(engine);

	ImGuiIO io = ImGui::GetIO();

    engine.RunEditor();

	editorRender.QuitImGui();

	return 0;
}


