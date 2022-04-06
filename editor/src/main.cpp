#include "game/entity.hpp"
#include "game/camera_component.hpp"
#include "game/entity_manager.hpp"
#include "game/time_manager.hpp"
#include "editor/editor_render.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"

#include "engine.hpp"

#undef main
int main()
{
    Engine engine;

	Editor::EditorRender editorRender{};
	editorRender.InitImGui();

	ImGuiIO io = ImGui::GetIO();

    UpdateEvent editorUpdateEvent = [&](){editorRender.UpdateAndRender(*engine.editorFBO, engine.entityManager, *engine.gameFBO);};
    engine.updateEventsHandler.emplace_back(editorUpdateEvent);

    engine.Run();

	editorRender.QuitImGui();

	return 0;
}
