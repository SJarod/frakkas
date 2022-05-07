#include "engine.hpp"

#include "game/entity.hpp"
#include "game/entity_manager.hpp"

#include "game/lowcomponent/static_draw.hpp"
#include "game/lowcomponent/animated_draw.hpp"
#include "game/lowcomponent/collider/box_collider.hpp"
#include "game/lowcomponent/collider/sphere_collider.hpp"
#include "game/bar.hpp"
#include "game/fps_movement.hpp"
#include "game/lowcomponent/camera_component.hpp"
#include "game/lowcomponent/sound_component.hpp"

#include "game/world.hpp"

//#define TEST_ANIMATION

using namespace Game;

const char* World::defaultScene = "default.kk";

World::World(Engine& engine)
{
	// ADD DEFAULT AXIS AND BUTTON
	Game::Inputs::SetButtonAction("quit", { Game::EButton::ESCAPE });
	Game::Inputs::SetButtonAction("pause", { Game::EButton::P });
	Game::Inputs::SetAxisAction("horizontal", { Game::EButton::ARROW_RIGHT, Game::EButton::D }, { Game::EButton::ARROW_LEFT, Game::EButton::A });
	Game::Inputs::SetAxisAction("vertical", { Game::EButton::SPACE }, { Game::EButton::LEFT_CTRL });
	Game::Inputs::SetAxisAction("forward", { Game::EButton::ARROW_UP, Game::EButton::W }, { Game::EButton::ARROW_DOWN, Game::EButton::S });

	EntityManager& em = engine.entityManager;
	Game::Entity* entity = nullptr;
	// CAMERA ENTITY
#if true
	{
		entity = em.CreateEntity("Camera entity");
		entity->transform.position = Vector3(0.f, 2.f, 5.f);
		entity->AddComponent<CameraComponent>();
        entity->AddComponent<FPSMovement>();
	}
#endif

	// DRAWABLE ENTITY WITH COLLIDER
#if true
	{
		entity = em.CreateEntity("Bagpack");

		auto staticDraw = entity->AddComponent<StaticDraw>();
		auto& model = staticDraw->model;
		model.SetMeshFromFile("game/assets/bp.fbx", "game/assets/bp.jpg", false);
		model.transform.scale = Vector3(0.01f, 0.01f, 0.01f);

		auto collider = entity->AddComponent<SphereCollider>();
		collider->isStatic = false;
	}
#endif

	// DRAWABLE ENTITY WITH COLLIDER AND VELOCITY
#if true
	{
		entity = em.CreateEntity("Super Bagpack");
		entity->transform.position = Vector3(0.f, 5.f, 0.f);

		auto staticDraw = entity->AddComponent<StaticDraw>();
		auto& model = staticDraw->model;
		model.SetMeshFromFile("game/assets/bp.fbx", "game/assets/bp.jpg", false);
		model.transform.scale = Vector3(0.01f, 0.01f, 0.01f);

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
		collider->halfExtension = Vector3(10.f, 1.f, 10.f);

		entity->transform.position = Vector3(0.f, -3.f, 0.f);

        entity->AddComponent<Bar>();
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
