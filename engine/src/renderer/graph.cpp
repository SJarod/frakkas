#include <cassert>

#include "game/component.hpp"
#include "game/drawable.hpp"
#include "game/light_component.hpp"
#include "game/camera_component.hpp"
#include "game/entity_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"

#include "resources/resources_manager.hpp"
#include "resources/serializer.hpp"

#include "renderer/graph.hpp"

using namespace Game;
using namespace Renderer;

std::queue<Game::Component*> Graph::entityComponentRegistry;

Graph::Graph(Game::EntityManager* entityManager)
    :entityManager(entityManager)
{}

void Graph::CheckComponentQueue() noexcept
{
    while(!entityComponentRegistry.empty())
    {
        Component* comp = entityComponentRegistry.front();
        if (comp->GetID() == Drawable::metaData.className)
            renderEntities.emplace_back(reinterpret_cast<Drawable*>(comp));
        else if (comp->GetID() == CameraComponent::metaData.className)
            gameCameras.emplace_back(reinterpret_cast<CameraComponent*>(comp));
        else if (comp->GetID() == LightComponent::metaData.className)
            lights.emplace_back(reinterpret_cast<LightComponent*>(comp));

        entityComponentRegistry.pop();
    }
}

void Graph::SetGameCameraAuto() noexcept
{
    for (CameraComponent* cameraComponent : gameCameras)
    {
        if (cameraComponent->enabled)
        {
            gameCamera = cameraComponent;
            return;
        }
    }

    gameCamera = nullptr;
}

void Graph::RenderEditor(Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio)
{
    UpdateGlobalUniform(i_renderer, i_aspectRatio, editorCamera);
    Render(i_renderer);
}

void Graph::RenderGame(Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio)
{
    if (!gameCamera || !gameCamera->enabled)
    {
        SetGameCameraAuto();
        return;
    }

    UpdateGlobalUniform(i_renderer, i_aspectRatio, gameCamera->camera);
    Render(i_renderer);
}

void Graph::Render(Renderer::LowLevel::LowRenderer& i_renderer)
{
    CheckComponentQueue();

    for (Drawable* drawable : renderEntities)
    {
        if (drawable && drawable->enabled)
        {
            const Renderer::Model& model = drawable->model;

            for (auto& mesh : model.meshes)
            {
                if (mesh == nullptr || mesh->gpu.VAO == 0)
                    continue;

                GLuint texToBeBinded = ResourcesManager::GetDefaultTexture().data;

                Resources::Texture* diffuseTex = mesh->diffuseTex.get();
                if (diffuseTex != nullptr)
                    if (diffuseTex->gpu)
                        texToBeBinded = diffuseTex->gpu->data;

                assert(mesh->gpu.VAO != 0);
                i_renderer.RenderMeshOnce(mesh->localTransform * model.transform.GetModelMatrix(),
                                          mesh->gpu.VAO,
                                          mesh->vertices.size(),
                                          texToBeBinded,
                                          true,
                                          lights.back()->light.outline);
            }
        }

    }
}

void Graph::UpdateGlobalUniform(const Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio,
                                Renderer::LowLevel::Camera& i_camera) const noexcept
{
// An entity with CameraComponent should be added to render
    i_renderer.SetUniform("uProjection", editorCamera.GetProjectionMatrix(i_aspectRatio));
    i_renderer.SetUniform("uView", i_camera.GetViewMatrix());
    i_renderer.SetUniform("uCameraView", i_camera.transform.position.get());

    if (lights.empty())
    {
        i_renderer.SetUniform("uLight.enabled", false);
        return;
    }
    // Use last light as default
    LightComponent* lightComp = lights.back();
    i_renderer.SetUniform("uLight.enabled", lightComp->enabled.get());
    auto& light = lightComp->light;
    i_renderer.SetUniform("uLight.position", light.position + lightComp->owner.get()->transform.position.get());
    i_renderer.SetUniform("uLight.ambient", light.ambient);
    i_renderer.SetUniform("uLight.diffuse", light.diffuse);
    i_renderer.SetUniform("uLight.specular", light.specular);

    i_renderer.SetUniform("uToonShading", lights.back()->light.toonShading);
    i_renderer.SetUniform("uFiveTone", lights.back()->light.fiveTone);
}

void Graph::ReloadScene()
{
    LoadScene(std::string(currentSceneName));
}

void Graph::LoadScene(const std::string& i_sceneName)
{
    std::string_view lastScene = currentSceneName;
    currentSceneName = i_sceneName;

    std::ifstream file(GetFullPath());

    if (file.is_open())
    {
        entityManager->CreateEntities(file);

        // Reset all lists
        gameCameras.clear();
        lights.clear();
        renderEntities.clear();
        // Sort registered component
        CheckComponentQueue();

        SetGameCameraAuto();
    }
    else
    {
        Log::Warning("Try to load scene from file '" + std::string(currentSceneName) + "' that does not exist.");
        currentSceneName = lastScene;
    }
}

void Graph::SaveScene() const
{
    std::ofstream file(GetFullPath());
    for (const std::unique_ptr<Entity>& entity : entityManager->GetEntities())
        Serializer::Write(file, *entity);
}

std::string Graph::GetFullPath() const
{
    return pathToScenes + std::string(currentSceneName) + ".kk";
}
