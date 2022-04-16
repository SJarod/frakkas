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

void EntityManager::Render(Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio)
{
    UpdateGlobalUniform(i_renderer, i_aspectRatio);

    i_renderer.SetUniform("uToonShading", lights.back()->light.toonShading);
    i_renderer.SetUniform("uFiveTone", lights.back()->light.fiveTone);

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
                                          true,
                                          lights.back()->light.outline);
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

Entity* EntityManager::CreateEntity(const std::string_view& i_name)
{
    entities.emplace_back(std::make_unique<Entity>(i_name));
    return entities.back().get();
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

    UpdateGlobalUniform(i_renderer, i_aspectRatio);

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
                                              true,
                                              lights.back()->light.outline);
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

void EntityManager::UpdateGlobalUniform(const LowRenderer& i_renderer, float i_aspectRatio) const noexcept
{
// An entity with CameraComponent should be added to render
    i_renderer.SetUniform("uProjection", editorCamera.GetProjectionMatrix(i_aspectRatio));
    i_renderer.SetUniform("uView", editorCamera.GetViewMatrix());
    i_renderer.SetUniform("uCameraView", editorCamera.transform.position.get());

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
}
