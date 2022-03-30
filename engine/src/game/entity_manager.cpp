//
// Created by flory on 3/17/2022.
//

#include <imgui.h>

#include "resources/mesh.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"

#include "game/entity.hpp"
#include "game/drawable.hpp"
#include "game/entity_manager.hpp"

using namespace Game;

void EntityManager::Update()
{
    for (const auto& entity : entities)
    {
        for (const std::shared_ptr<Component>& comp : entity->components)
            if (comp->IsEnabled())
                comp->Update();
    }
}

void EntityManager::Render(Renderer::LowLevel::LowRenderer &i_renderer, const Renderer::LowLevel::Camera &i_camera, const float i_aspectRatio)
{
    i_renderer.SetProjection(i_camera.GetProjectionMatrix(i_aspectRatio));
    i_renderer.SetView(i_camera.GetViewMatrix());

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
    for (const std::shared_ptr<Component>& comp : i_entity->components)
        comp->Start();

    entities.push_back(std::move(i_entity));
}

const std::list<std::unique_ptr<Entity>> & EntityManager::GetEntities() const {
    return entities;
}


