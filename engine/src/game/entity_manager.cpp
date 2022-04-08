#include "resources/mesh.hpp"
#include "resources/serializer.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"
#include "game/entity.hpp"
#include "game/drawable.hpp"
#include "game/camera_component.hpp"

#include "game/entity_manager.hpp"

using namespace Game;

EntityManager::EntityManager()
{
    editorCamera.transform.position = Vector3(0.f, 0.f, 5.f);
}

void EntityManager::Update()
{
    for (const auto& entity : entities)
    {
        for (const std::unique_ptr<Component>& comp: entity->components)
        {
            if (comp->enabled)
                comp->Update();
        }
    }
}

void EntityManager::Render(Renderer::LowLevel::LowRenderer& i_renderer, const float i_aspectRatio)
{
    // An entity with CameraComponent should be added to render
    i_renderer.SetProjection(editorCamera.GetProjectionMatrix(i_aspectRatio));
    i_renderer.SetView(editorCamera.GetViewMatrix());

    for (const auto& entity : entities)
    {
        auto drawable = entity->GetComponent<Drawable>("drawable");
        if (drawable && drawable->enabled)
        {
            for (const std::shared_ptr<Resources::Mesh> mesh : drawable->model.meshes)
            {
                i_renderer.RenderMeshOnce(mesh->localTransform * drawable->model.transform.GetModelMatrix(),
                                          mesh->gpu.VAO,
                                          mesh->vertices.size(),
                                          mesh->diffuseTex.data,
                                          true);
            }
        }

    }
}

void EntityManager::AddEntity(std::unique_ptr<Entity> i_entity)
{
    for (const std::unique_ptr<Component>& comp : i_entity->components)
    {
        comp->Start();
        if (!gameCamera)
        {
            if (CameraComponent* cameraEntity = reinterpret_cast<CameraComponent*>(comp.get()))
                gameCamera = cameraEntity;
        }
    }

    entities.push_back(std::move(i_entity));
}

const std::list<std::unique_ptr<Entity>> & EntityManager::GetEntities() const
{
    return entities;
}

void EntityManager::UpdateAndRender(LowRenderer &i_renderer, const float i_aspectRatio) {
    // An entity with CameraComponent should be added to render
    if (!gameCamera || !gameCamera->enabled)
    {
        FindGameCamera();
        return;
    }

    i_renderer.SetProjection(gameCamera->camera.GetProjectionMatrix(i_aspectRatio));
    i_renderer.SetView(gameCamera->camera.GetViewMatrix());

    for (const auto& entity : entities)
    {
        for (const std::unique_ptr<Component>& comp : entity->components)
        {
            if (!comp->enabled)
                continue;

            comp->Update();

            if (comp->id == "drawable")
            {
                Drawable* drawable = reinterpret_cast<Drawable*>(comp.get());
                for (const std::shared_ptr<Resources::Mesh>& mesh : drawable->model.meshes)
                {
                    i_renderer.RenderMeshOnce(mesh->localTransform * drawable->model.transform.GetModelMatrix(),
                                              mesh->gpu.VAO,
                                              mesh->vertices.size(),
                                              mesh->diffuseTex.data,
                                              true);
                }
            }
        }
    }
}

void EntityManager::FindGameCamera() noexcept
{
    for (std::unique_ptr<Entity>& entity: entities)
    {
        if (CameraComponent* cameraComponent = entity->GetComponent<CameraComponent>("camera"))
        {
            if (cameraComponent->enabled)
            {
                gameCamera = cameraComponent;
                return;
            }
        }
    }

    gameCamera = nullptr;
}

Entity* EntityManager::CreateEntity()
{
    entities.push_back(std::make_unique<Entity>());
    return entities.back().get();
}

