//
// Created by flory on 3/17/2022.
//

#include <imgui.h>

#include "game/engine_entity.hpp"
#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"

#include "resources/mesh.hpp"

#include "game/entity_manager.hpp"

using namespace Game;

void EntityManager::Update()
{
    for (const auto& entity : entities)
    {
        entity->Update();
    }
}

void EntityManager::Render(Renderer::LowLevel::LowRenderer &i_renderer, const Renderer::LowLevel::Camera &i_camera, const float i_aspectRatio)
{
    i_renderer.SetProjection(i_camera.GetProjectionMatrix(i_aspectRatio));
    i_renderer.SetView(i_camera.GetViewMatrix());

    for (const auto& entity : entities)
    {
        const Renderer::Model& model = entity->GetModel();

        for (const std::shared_ptr<Resources::Mesh> mesh : model.meshes)
        {
            i_renderer.RenderMeshOnce(mesh->localTransform * model.transform.GetModelMatrix(),
                                      mesh->gpu.VAO,
                                      mesh->vertices.size(),
                                      mesh->diffuseTex.data,
                                      true);
        }
    }
}

void EntityManager::AddEntity(std::unique_ptr<EngineEntity> i_entity)
{
    i_entity->Start();
    entities.push_back(std::move(i_entity));
}

const std::vector<std::unique_ptr<EngineEntity>> &EntityManager::GetEntities() const {
    return entities;
}


