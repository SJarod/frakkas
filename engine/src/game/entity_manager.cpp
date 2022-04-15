#include "resources/mesh.hpp"
#include "resources/serializer.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"
#include "game/entity.hpp"
#include "game/drawable.hpp"
#include "game/camera_component.hpp"
#include "game/light_component.hpp"

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
    i_renderer.SetUniform("uProjection", editorCamera.GetProjectionMatrix(i_aspectRatio));
    i_renderer.SetUniform("uView", editorCamera.GetViewMatrix());
    i_renderer.SetUniform("uCameraView", editorCamera.transform.position.get());

    i_renderer.SetUniform("uLight.enabled", lights.back()->enabled.get());
    i_renderer.SetUniform("uLight.position", lights.back()->light.position);
    i_renderer.SetUniform("uLight.ambient", lights.back()->light.ambient);
    i_renderer.SetUniform("uLight.diffuse", lights.back()->light.diffuse);
    i_renderer.SetUniform("uLight.specular", lights.back()->light.specular);

    for (const auto& entity : entities)
    {
        auto drawable = entity->GetComponent<Drawable>();
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

    entities.emplace_back(std::move(i_entity));
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

    i_renderer.SetUniform("uProjection", gameCamera->camera.GetProjectionMatrix(i_aspectRatio));
    i_renderer.SetUniform("uView", gameCamera->camera.GetViewMatrix());

    //i_renderer.SetProjection(gameCamera->camera.GetProjectionMatrix(i_aspectRatio));
    //i_renderer.SetView(gameCamera->camera.GetViewMatrix());

    for (const auto& entity : entities)
    {
        for (const std::unique_ptr<Component>& comp : entity->components)
        {
            if (!comp->enabled)
                continue;

            comp->Update();

            if (comp->GetID() == Drawable::id)
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
        if (CameraComponent* cameraComponent = entity->GetComponent<CameraComponent>())
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

void EntityManager::FindLight() noexcept
{
    lights.clear();

    for (std::unique_ptr<Entity>& entity: entities)
    {
        if (LightComponent* lightComponent = entity->GetComponent<LightComponent>())
            lights.emplace_back(lightComponent);
    }
}

Entity* EntityManager::CreateEntity()
{
    entities.emplace_back(std::make_unique<Entity>());
    return entities.back().get();
}
