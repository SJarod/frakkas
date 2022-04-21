#include "game/entity.hpp"
#include "game/camera_component.hpp"
#include "game/inputs_manager.hpp"
#include "game/time_manager.hpp"

#include "editor/editor_render.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"

#include "engine.hpp"

#if 1
// Run on laptop high perf GPU
extern "C"
{
    __declspec(dllexport) int NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

struct Test{ int  i = 0;};

#define TEA(lol)

#undef main
int main()
{
    Engine engine;

	Editor::EditorRender editorRender{};
	editorRender.InitImGui();

	ImGuiIO io = ImGui::GetIO();

    UpdateEvent editorUpdateEvent = [&]()
            {
                editorRender.UpdateAndRender(*engine.editorFBO, engine.entityManager, *engine.gameFBO);
            };
    UpdateEvent editorCameraUpdateEvent = [&]()
            {
                Renderer::LowLevel::Camera& cam = engine.entityManager.editorCamera;
                float speed = 20.f * Game::Time::GetDeltaTime();

                Vector3 pos = cam.transform.position;
                    pos.x += speed * Game::Inputs::GetAxis("horizontal");
                    pos.y += speed * Game::Inputs::GetAxis("vertical");
                    pos.z += speed * -Game::Inputs::GetAxis("forward");
                cam.transform.position = pos;
            };

    engine.updateEventsHandler.emplace_back(editorUpdateEvent);
    engine.updateEventsHandler.emplace_back(editorCameraUpdateEvent);

    engine.Run();

	editorRender.QuitImGui();

	return 0;
}
