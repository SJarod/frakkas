#include <cassert>

#include "game/lowcomponent/component.hpp"
#include "game/lowcomponent/drawable.hpp"
#include "game/lowcomponent/camera.hpp"
#include "game/lowcomponent/collider/box_collider.hpp"
#include "game/lowcomponent/collider/sphere_collider.hpp"
#include "game/entity_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"

#include "resources/resources_manager.hpp"
#include "resources/serializer.hpp"

#include "physic/physic_scene.hpp"

#include "helpers/path_constants.hpp"

#include "renderer/graph.hpp"

using namespace Game;
using namespace Renderer;

Physic::PhysicScene* Graph::physicScene = nullptr;

std::vector<Game::Camera*> Graph::gameCameras;
std::vector<Game::Drawable*> Graph::renderEntities;
bool Graph::updateCamera = true;

Graph::Graph(Game::EntityManager* io_entityManager, Physic::PhysicScene* i_physicScene)
    :entityManager(io_entityManager), editorCameraman(0, "Editor cameraman")
{
    physicScene = i_physicScene;
    editorCameraman.transform.position = Vector3::forward * 4.f;
    editorCamera = editorCameraman.AddComponent<Game::Camera>();
}

void Graph::RegisterComponent(Game::Component* i_newComponent)
{
    std::string ID = i_newComponent->GetID();
    if (ID == "StaticDraw" || ID == "AnimatedDraw")
        renderEntities.emplace_back(reinterpret_cast<Drawable*>(i_newComponent));
    else if (ID == Game::Camera::MetaData().className)
        gameCameras.emplace_back(reinterpret_cast<Game::Camera*>(i_newComponent));
    else if (ID == BoxCollider::MetaData().className)
    {
        auto box = reinterpret_cast<BoxCollider*>(i_newComponent);
        box->SetCollider(physicScene->CreateBody(Vector3(1.f, 1.f, 1.f)));
        Physic::PhysicScene::colliders.emplace_back(box);
    }
    else if (ID == SphereCollider::MetaData().className)
    {
        auto sphere = reinterpret_cast<SphereCollider*>(i_newComponent);
        sphere->SetCollider(physicScene->CreateBody(1.f));
        Physic::PhysicScene::colliders.emplace_back(sphere);
    }
}

void Graph::UnregisterComponent(Game::Component* i_oldComponent)
{
    std::string ID = i_oldComponent->GetID();
    if (ID == "StaticDraw" || ID == "AnimatedDraw")
    {
        auto it = std::find(renderEntities.begin(), renderEntities.end(), reinterpret_cast<Drawable*>(i_oldComponent));
        if (it != renderEntities.end())
            renderEntities.erase(it);
    }
    else if (ID == Game::Camera::MetaData().className)
    {
        auto it = std::find(gameCameras.begin(), gameCameras.end(), reinterpret_cast<Game::Camera*>(i_oldComponent));
        if (it != gameCameras.end())
            gameCameras.erase(it);
        updateCamera = true;
    }
    else if (ID == BoxCollider::MetaData().className || ID == SphereCollider::MetaData().className)
    {
        auto collider = reinterpret_cast<Collider*>(i_oldComponent);
        physicScene->RemoveBody(collider->GetPhysicBodyID());
    }
}

void Graph::SetGameCameraAuto() noexcept
{
    updateCamera = false;
    for (Game::Camera* cameraComponent : gameCameras)
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
    UpdateGlobalUniform(i_renderer,
        i_aspectRatio,
        *editorCamera,
        editorCameraman.transform.position);
    Render(i_renderer);
    RenderColliders(i_renderer);
}

void Graph::RenderGame(Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio)
{
    if (updateCamera || !gameCamera || !gameCamera->enabled)
    {
        SetGameCameraAuto();
        return;
    }

    UpdateGlobalUniform(i_renderer,
        i_aspectRatio,
        *gameCamera,
        gameCamera->owner.get()->transform.position);
    Render(i_renderer);
}

void Graph::Render(Renderer::LowLevel::LowRenderer& i_renderer)
{
    // light depth map
    i_renderer.BeginFrame(*i_renderer.depthMapFBO);

    for (Drawable* drawable : renderEntities)
    {
        if (drawable && drawable->enabled)
            drawable->DrawDepthMap(i_renderer, drawable->owner.get()->transform);
    }

    i_renderer.EndFrame();

    // normal rendering
    i_renderer.BeginFrame(*i_renderer.firstPassFBO);

    for (Drawable* drawable : renderEntities)
    {
        if (drawable && drawable->enabled)
            drawable->Draw(i_renderer, light, drawable->owner.get()->transform);
    }

    i_renderer.EndFrame();

    // post processing
    i_renderer.BeginFrame(*i_renderer.secondPassFBO);

    i_renderer.EndFrame();
}

void Graph::RenderColliders(Renderer::LowLevel::LowRenderer& i_renderer)
{
    i_renderer.ContinueFrame(*i_renderer.firstPassFBO);

    for (Collider* collider : Physic::PhysicScene::colliders)
    {
        if (collider && collider->enabled)
            collider->DebugDraw(i_renderer, collider->owner.get()->transform);
    }

    i_renderer.EndFrame();
}

void Graph::UpdateGlobalUniform(const Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio, Game::Camera& i_camera, const Vector3& i_cameraPos) const noexcept
{
// An entity with Camera should be added to render
    // projection
    i_renderer.SetUniformToNamedBlock("uRenderMatrices", 0, i_camera.GetProjectionMatrix(i_aspectRatio));
    // view
    i_renderer.SetUniformToNamedBlock("uRenderMatrices", 64, i_camera.GetViewMatrix());

    float shadowRange = i_renderer.shadowRange;
    float shadowDepth = i_renderer.shadowDepth;
    // lightProjection
    i_renderer.SetUniformToNamedBlock("uLightMatrices", 0, Matrix4::Orthographic(-shadowRange, shadowRange, -shadowRange, shadowRange, -shadowDepth, shadowDepth));
    // lightView
    i_renderer.SetUniformToNamedBlock("uLightMatrices", 64, Matrix4::LookAt(Vector3(light.position) + i_cameraPos, i_cameraPos, Vector3::up));
    // cameraPos
    i_renderer.SetUniformToNamedBlock("uRendering", 112, i_cameraPos);

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
    // stepAmount
    i_renderer.SetUniformToNamedBlock("uRendering", 84, light.stepAmount);
    // stepSize
    i_renderer.SetUniformToNamedBlock("uRendering", 88, light.stepSize);
    // specSize
    i_renderer.SetUniformToNamedBlock("uRendering", 92, light.specSize);

    // shadow
    i_renderer.SetUniformToNamedBlock("uRendering", 96, light.shadow);
    // adaptativeBias
    i_renderer.SetUniformToNamedBlock("uRendering", 100, light.adaptativeBias);
    // shadowBias
    i_renderer.SetUniformToNamedBlock("uRendering", 104, light.shadowBias);
}

void Graph::ReloadScene()
{
    LoadScene(currentScenePath);
}

void Graph::LoadScene(const std::filesystem::path& i_scenePath)
{
    std::filesystem::path lastScene = currentScenePath;
    currentScenePath = i_scenePath;

    std::ifstream file(currentScenePath);

    if (file.is_open())
    {
        // Reset all lists
        gameCameras.clear();
        renderEntities.clear();
        physicScene->Clear();

        // Parse file to create entities
        entityManager->LoadEntities(file);

        SetGameCameraAuto();

        Log::Info("Load scene ", currentScenePath);
    }
    else
    {
        Log::Warning("Try to load scene from file '", currentScenePath, "' that does not exist.");
        currentScenePath = lastScene;
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
    std::ofstream file(currentScenePath);

    for (const auto& pair : entityManager->GetRootEntities())
        SaveEntity(file, *pair.second);

    Log::Info("Save scene ", currentScenePath);
}

std::filesystem::path Graph::GetSceneFullPath(const std::string& i_sceneName)
{
    return Helpers::gameDirectoryPath + std::string("assets/") + i_sceneName + ".kk";
}
