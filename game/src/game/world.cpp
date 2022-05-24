#include "engine.hpp"

#include "renderer/graph.hpp"

#include "game/entity.hpp"
#include "game/entity_manager.hpp"

#include "game/lowcomponent/static_draw.hpp"
#include "game/lowcomponent/animated_draw.hpp"
#include "game/lowcomponent/collider/box_collider.hpp"
#include "game/lowcomponent/collider/sphere_collider.hpp"
#include "fps_movement.hpp"
#include "game/lowcomponent/camera.hpp"

#include "game/world.hpp"

//#define TEST_ANIMATION

using namespace Game;

World::World(Engine& engine)
{
	// ADD DEFAULT AXIS AND BUTTON
	Inputs::SetAxisAction("horizontal",
                                { EButton::ARROW_RIGHT, EButton::D, EButton::GAMEPAD_DPAD_RIGHT },
                                { EButton::ARROW_LEFT, EButton::A, EButton::GAMEPAD_DPAD_LEFT });

	Inputs::SetAxisAction("vertical",
                          { EButton::SPACE }, { EButton::LEFT_CTRL });

    Inputs::SetAxisAction("verticalDebug",
                          { EButton::SPACE, EButton::E }, { EButton::LEFT_CTRL, EButton::Q });

	Inputs::SetAxisAction("forward",
                          { EButton::ARROW_UP, EButton::W, EButton::GAMEPAD_DPAD_UP },
                          { EButton::ARROW_DOWN, EButton::S, EButton::GAMEPAD_DPAD_DOWN });

#if false
    {

        EntityManager& em = engine.entityManager;
        Entity* entity = nullptr;
        // CAMERA ENTITY
    #if true
        {
            entity = em.CreateEntity("Camera entity");
            entity->transform.position = Vector3(0.f, 2.f, 5.f);
            entity->AddComponent<Camera>();
            entity->AddComponent<FPSMovement>();
        }
    #endif

        // DRAWABLE ENTITY WITH COLLIDER
    #if true
        {
            entity = em.CreateEntity("Bagpack");
            entity->transform.scale = Vector3(0.01f, 0.01f, 0.01f);

            auto staticDraw = entity->AddComponent<StaticDraw>();
            staticDraw->model.SetMeshFromFile("game/assets/bp.fbx", "game/assets/bp.jpg", false);

            auto collider = entity->AddComponent<SphereCollider>();
            collider->isStatic = false;
        }
    #endif

        // DRAWABLE ENTITY WITH COLLIDER AND VELOCITY
    #if true
        {
            entity = em.CreateEntity("Super Bagpack");
            entity->transform.position = Vector3(0.f, 5.f, 0.f);
            entity->transform.scale = Vector3(0.01f, 0.01f, 0.01f);

            auto staticDraw = entity->AddComponent<StaticDraw>();
            staticDraw->model.SetMeshFromFile("game/assets/bp.fbx", "game/assets/bp.jpg", false);

            auto collider = entity->AddComponent<SphereCollider>();
            collider->velocity = { 0.f, -5.f, 0.f };
            collider->isStatic = false;
        }
    #endif

        // DRAWABLE ENTITY 2
    #if true
        {
            entity = em.CreateEntity("Banana");
            auto animated = entity->AddComponent<Game::AnimatedDraw>();
            auto& skmodel = animated->skmodel;
            skmodel.SetSkeletalMeshFromFile("game/assets/fortnite.fbx", "game/assets/fortnite.png", false);
            skmodel.LoadAnimationsForThis("game/assets/fortnite.fbx");
        }
    #endif

        // GROUND ENTITY
    #if true
        {
            entity = em.CreateEntity("Ground");
            auto collider = entity->AddComponent<BoxCollider>();
            entity->transform.scale = Vector3(10.f, 1.f, 10.f);

            entity->transform.position = Vector3(0.f, -3.f, 0.f);
        }
    #endif

    #if false
        for (int i = 0; i < 10; i++)
        {
            Game::Entity* entity = engine.entityManager.CreateEntity("skmesh" + std::to_string(i));
            entity->transform.position = Vector3(i * 2.f, 0.f, 0.f);
            entity->transform.rotation = Vector3(-Maths::Constants::halfPi, 0.f, 0.f);
            entity->transform.scale = Vector3(0.1f, 0.1f, 0.1f);

            auto animated = entity->AddComponent<Game::AnimatedDraw>();
            auto& skmodel = animated->skmodel;
            skmodel.SetSkeletalMeshFromFile("game/assets/fortnite.fbx", "game/assets/fortnite.png", false);
            skmodel.LoadAnimationsForThis("game/assets/fortnite.fbx");
        }
    #endif
    }
#else
    engine.graph->ReloadScene();
#endif
}
