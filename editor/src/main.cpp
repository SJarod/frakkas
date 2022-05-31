#include "editor/editor_render.hpp"

#include "game/register_components.hpp"
#include "game/world.hpp"

#include "engine.hpp"

#undef main
int main()
{
    Engine engine;

    Game::RegisterComponents(engine.entityManager);

	Editor::EditorRender editorRender(engine);

	ImGuiIO io = ImGui::GetIO();

    engine.RunEditor();

	return 0;
}


