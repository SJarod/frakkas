//
// Created by flory on 3/17/2022.
//

#include "engine/engine_entity.hpp"
#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"

#include "engine/entity_manager.hpp"

using namespace Engine;

void EntityManager::Update() {

    for (const auto& entity : entities)
    {
        entity->Update();
    }
}

void EntityManager::Render(Renderer::LowLevel::LowRenderer &i_renderer, const Renderer::LowLevel::Camera &i_camera) {

    for (const auto& entity : entities)
    {
        i_renderer.RenderModelOnce(entity->GetModel(),
                                   i_camera.GetViewMatrix(),
                                   i_camera.GetProjectionMatrix(1000.f / 1000.f));
    }

}

void EntityManager::AddEntity(std::unique_ptr<EngineEntity> i_entity) {
    i_entity->Start();
    entities.push_back(std::move(i_entity));
}
