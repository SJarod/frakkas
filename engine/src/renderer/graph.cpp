#include <cassert>

#include "game/lowcomponent/component.hpp"
#include "game/lowcomponent/drawable.hpp"
#include "game/lowcomponent/camera_component.hpp"
#include "game/entity_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"

#include "resources/resources_manager.hpp"
#include "resources/serializer.hpp"

#include "helpers/path_constants.hpp"

#include "renderer/graph.hpp"

using namespace Game;
using namespace Renderer;

std::vector<Game::CameraComponent*> Graph::gameCameras;
std::vector<Game::Drawable*> Graph::renderEntities;
bool Graph::updateCamera = true;

Graph::Graph(Game::EntityManager* entityManager)
    :entityManager(entityManager)
{}

void Graph::RegisterComponent(Game::Component* i_newComponent)
{
    if (i_newComponent->GetID() == "StaticDraw" || i_newComponent->GetID() == "AnimatedDraw")
        renderEntities.emplace_back(reinterpret_cast<Drawable*>(i_newComponent));
    else if (i_newComponent->GetID() == CameraComponent::metaData.className)
        gameCameras.emplace_back(reinterpret_cast<CameraComponent*>(i_newComponent));
}

void Graph::UnregisterComponent(Game::Component* i_oldComponent)
{
    if (i_oldComponent->GetID() == "StaticDraw" || i_oldComponent->GetID() == "AnimatedDraw")
    {
        auto it = std::find(renderEntities.begin(), renderEntities.end(), reinterpret_cast<Drawable*>(i_oldComponent));
        if (it != renderEntities.end())
            renderEntities.erase(it);
    }
    else if (i_oldComponent->GetID() == CameraComponent::metaData.className)
    {
        auto it = std::find(gameCameras.begin(), gameCameras.end(), reinterpret_cast<CameraComponent*>(i_oldComponent));
        if (it != gameCameras.end())
            gameCameras.erase(it);
        updateCamera = true;
    }
}

void Graph::SetGameCameraAuto() noexcept
{
    updateCamera = false;
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
    if (updateCamera || !gameCamera || !gameCamera->enabled)
    {
        SetGameCameraAuto();
        return;
    }

    UpdateGlobalUniform(i_renderer, i_aspectRatio, gameCamera->camera);
    Render(i_renderer);
}

void Graph::Render(Renderer::LowLevel::LowRenderer& i_renderer)
{
    for (Drawable* drawable : renderEntities)
    {
        if (drawable && drawable->enabled)
            drawable->Draw(i_renderer, light);
    }
}

void Graph::UpdateGlobalUniform(const Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio,
                                Renderer::LowLevel::Camera& i_camera) const noexcept
{
// An entity with CameraComponent should be added to render
    // projection
    i_renderer.SetUniformToNamedBlock("uProjView", 0, editorCamera.GetProjectionMatrix(i_aspectRatio));
    // view
    i_renderer.SetUniformToNamedBlock("uProjView", 64, i_camera.GetViewMatrix());
    // cameraView
    i_renderer.SetUniformToNamedBlock("uRendering", 96, i_camera.transform.position.get());

    // Use last light as default
    // light.enabled
    i_renderer.SetUniformToNamedBlock("uRendering", 0, lightEnabled);
    // light.position
    i_renderer.SetUniformToNamedBlock("uRendering", 16, light.position);
    // light.ambient
    i_renderer.SetUniformToNamedBlock("uRendering", 32, light.ambient);
    // light.diffuse
    i_renderer.SetUniformToNamedBlock("uRendering", 48, light.diffuse);
    // light.specular
    i_renderer.SetUniformToNamedBlock("uRendering", 64, light.specular);

    // toonShading
    i_renderer.SetUniformToNamedBlock("uRendering", 80, light.toonShading);
    // fiveTone
    i_renderer.SetUniformToNamedBlock("uRendering", 88, light.fiveTone);
}

void Graph::ReloadScene()
{
    LoadScene(std::string(currentSceneName));
}

void Graph::LoadScene(const std::string& i_sceneName)
{
    std::string_view lastScene = currentSceneName;
    currentSceneName = i_sceneName;

    std::ifstream file(GetSceneFullPath(currentSceneName));

    if (file.is_open())
    {
        // Reset all lists
        gameCameras.clear();
        renderEntities.clear();

        // Parse file to create entities
        entityManager->CreateEntities(file);

        SetGameCameraAuto();

        Log::Info("Load scene " + currentSceneName);
    }
    else
    {
        Log::Warning("Try to load scene from file '" + currentSceneName + "' that does not exist.");
        currentSceneName = lastScene;
    }
}

// Save an entity in text format, function to simplify SaveScene()
inline void SaveEntity(std::ofstream& i_file, const Entity& i_entity)
{
    Serializer::Write(i_file, i_entity);
    for (const Entity* child : i_entity.childs)
        SaveEntity(i_file, *child);
}

void Graph::SaveScene() const
{
    std::ofstream file(GetSceneFullPath(currentSceneName));

    for (const auto& pair : entityManager->GetRootEntities())
        SaveEntity(file, *pair.second);

    Log::Info("Save scene " + currentSceneName);
}

std::string Graph::GetSceneFullPath(const std::string& i_sceneName)
{
    return Helpers::gameDirectoryPath + std::string("assets/") + i_sceneName + ".kk";
}
