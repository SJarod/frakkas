#include "editor/editor_render.hpp"

#include "game/register_components.hpp"
#include "game/world_data.hpp"

#include "engine.hpp"

#undef main
int main()
{
    Engine engine;

    Game::RegisterComponents(engine.entityManager);

    Game::World::SetWorldData(std::make_unique<Game::FrakkarenaWorldData>());

	Editor::EditorRender editorRender(engine);

	ImGuiIO io = ImGui::GetIO();

    engine.RunEditor();

	return 0;
}


