#include <imgui.h>

#include "game/inputs_manager.hpp"

#include "renderer/lowlevel/camera.hpp"
#include "renderer/lowlevel/lowrenderer.hpp"

#include "engine.hpp"

#include "editor/scene.hpp"


using namespace Editor;


void Scene::OnImGuiRender(Renderer::LowLevel::Framebuffer& io_fbo, Renderer::LowLevel::Camera& camera)
{
    ImGui::Begin("Scene");

    CheckMouseAction();

	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	ImGui::Image(reinterpret_cast<ImTextureID>(io_fbo.GetColor0()), windowSize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    io_fbo.aspectRatio = windowSize.x / windowSize.y;

    float newFovY = 2.f * Maths::Atan(Maths::Tan(camera.targetFovY / io_fbo.aspectRatio * 0.5f) * io_fbo.aspectRatio);
    if (io_fbo.aspectRatio > 1.f)
        camera.SetFieldOfView(newFovY);
}

void Scene::CheckMouseAction()
{
    if (!isMoving)
    {
        isMoving = ImGui::IsWindowHovered() && Game::Inputs::IsPressed(Game::EButton::MOUSE_RIGHT);

        // If User click on editor scene window
        if (isMoving)
        {
            ImGui::SetWindowFocus(nullptr);
            Engine::SetCursorGameMode(true);
            mouseLockPosition = Game::Inputs::GetMousePosition();
        }
    }
    else
    {
        isMoving = !Game::Inputs::IsReleased(Game::EButton::MOUSE_RIGHT);

        // If User unClick
        if (!isMoving)
        {
            Engine::SetCursorGameMode(false);
            Engine::SetCursorPosition(mouseLockPosition);
        }
    }
}
