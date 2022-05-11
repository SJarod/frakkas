#include <imgui.h>

#include "game/inputs_manager.hpp"
#include "game/entity.hpp"
#include "game/lowcomponent/sound_component.hpp"
#include "game/lowcomponent/static_draw.hpp"

#include "renderer/graph.hpp"
#include "renderer/lowlevel/camera.hpp"
#include "renderer/lowlevel/lowrenderer.hpp"

#include "engine.hpp"

#include "editor/scene.hpp"


using namespace Editor;

void Scene::OnImGuiRender(Engine& io_engine, Game::Entity* i_selectedEntity, ImGuizmo::OPERATION& i_gizmoOperation)
{
    Renderer::LowLevel::Framebuffer& sceneFBO = *io_engine.editorFBO;
    Renderer::LowLevel::Camera& editorCamera = *io_engine.GetEditorGamera();

    ImGui::Begin("Scene");

    CheckMouseAction();

    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImGui::Image(reinterpret_cast<ImTextureID>(io_engine.editorFBO->GetColor0()), windowSize, ImVec2(0, 1), ImVec2(1, 0));

    DragDropResources(io_engine.entityManager, *io_engine.graph);

    if (!isMoving)
    {
        if(Game::Inputs::IsPressed(Game::EButton::Q))
            i_gizmoOperation = ImGuizmo::OPERATION::BOUNDS;

        else if(Game::Inputs::IsPressed(Game::EButton::W))
            i_gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

        else if(Game::Inputs::IsPressed(Game::EButton::E))
            i_gizmoOperation = ImGuizmo::OPERATION::ROTATE;

        else if(Game::Inputs::IsPressed(Game::EButton::R))
            i_gizmoOperation = ImGuizmo::OPERATION::SCALE;
    }

    if (i_selectedEntity && i_gizmoOperation != ImGuizmo::OPERATION::BOUNDS)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowSize.x, windowSize.y);

        Matrix4 cameraProj = editorCamera.GetProjectionMatrix(windowSize.x / windowSize.y);
        Matrix4 cameraView = editorCamera.GetViewMatrix();

        Game::Transform& trs = i_selectedEntity->transform;
        Matrix4 trsMatrix = i_selectedEntity->transform.GetModelMatrix();

        Matrix4 deltaMatrix = Matrix4::Identity();

        if(ImGuizmo::Manipulate(cameraView.element, cameraProj.element, i_gizmoOperation, ImGuizmo::LOCAL, trsMatrix.element, deltaMatrix.element))
        {
            /*
            Normally we could have done trsMatrix *= deltaMatrix, unfortunately it doesn't work.
            So we decide to decompose deltaMatrix. Even with this, the scale component didn't work,
             so we had to use the one from trsMatrix.
            This is why we decompose the two matrices.
            */

            Vector3 pos, scale, rot;
            Vector3 posDelta, scaleDelta, rotDelta;

            ImGuizmo::DecomposeMatrixToComponents(trsMatrix.element, pos.element, rot.element, scale.element);
            ImGuizmo::DecomposeMatrixToComponents(deltaMatrix.element, posDelta.element, rotDelta.element, scaleDelta.element);

            if (i_gizmoOperation == ImGuizmo::OPERATION::TRANSLATE)
                trs.position = trs.position.get() + posDelta;

            if (i_gizmoOperation == ImGuizmo::OPERATION::ROTATE)
                trs.rotation = trs.rotation.get() + Maths::ToRadians(rotDelta);

            if (i_gizmoOperation == ImGuizmo::OPERATION::SCALE)
                trs.scale = scale;
        }
    }

    ImGui::End();

    io_engine.editorFBO->aspectRatio = windowSize.x / windowSize.y;
}

void Scene::DragDropResources(Game::EntityManager& io_entityManager, Renderer::Graph& io_graph)
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_BROWSER_ITEM"))
        {
            std::filesystem::path path = *static_cast<std::filesystem::path*>(payload->Data);

            if (path.extension() == ".wav" || path.extension() == ".mp3")
            {
                Game::Entity* entity = io_entityManager.CreateEntity("Sound entity");
                entity->transform.position = Vector3::zero;
                auto soundComp = entity->AddComponent<Game::SoundComponent>();
                soundComp->sound.SetSound(path.string());
            }

            else if (path.extension() == ".obj" || path.extension() == ".fbx" || path.extension() == ".gltf")
            {
                Game::Entity* entity = io_entityManager.CreateEntity();

                auto staticDraw = entity->AddComponent<Game::StaticDraw>();
                staticDraw->model.SetMeshFromFile(path.string());
            }

            else if (path.extension() == ".kk" )
                io_graph.LoadScene(path.string());
        }

        ImGui::EndDragDropTarget();
    }
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
