#include "resources/mesh.hpp"
#include "resources/serializer.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"
#include "game/entity.hpp"
#include "game/drawable.hpp"
#include "game/camera_component.hpp"

#include "game/entity_manager.hpp"


using namespace Game;

void EntityManager::Update()
{
    for (const auto& entity : entities)
    {
        for (const std::unique_ptr<Component>& comp : entity->components)
        {
            if (comp->IsEnabled())
                comp->Update();
        }
    }
}

void EntityManager::Render(Renderer::LowLevel::LowRenderer& i_renderer, const float i_aspectRatio)
{
    // An entity with CameraComponent should be added to render
    if (!camera)
        return;

    i_renderer.SetProjection(camera->GetProjectionMatrix(i_aspectRatio));
    i_renderer.SetView(camera->GetViewMatrix());

    for (const auto& entity : entities)
    {
        auto drawable = entity->GetComponent<Drawable>("drawable");
        if (drawable)
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
        if (!camera)
        {
            if (CameraComponent* cameraEntity = reinterpret_cast<CameraComponent*>(comp.get()))
                camera = &cameraEntity->camera;
        }
    }

    entities.push_back(std::move(i_entity));
}

const std::list<std::unique_ptr<Entity>>& EntityManager::GetEntities() const
{
    return entities;
}