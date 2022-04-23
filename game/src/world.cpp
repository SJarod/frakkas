#include "engine.hpp"

#include "game/entity.hpp"
#include "game/entity_manager.hpp"

#include "game/drawable.hpp"
#include "game/bar.hpp"
#include "game/fps_movement.hpp"
#include "game/camera_component.hpp"
#include "game/sound_component.hpp"
#include "game/light_component.hpp"

#include "game/world.hpp"

using namespace Game;

const char* World::defaultScene = "default.kk";

World::World(Engine& engine)
{
    // ADD DEFAULT AXIS AND BUTTON
    Game::Inputs::SetButtonAction("quit", { Game::EButton::ESCAPE });
    Game::Inputs::SetButtonAction("pause", { Game::EButton::P});
    Game::Inputs::SetAxisAction("horizontal", { Game::EButton::ARROW_RIGHT, Game::EButton::D }, { Game::EButton::ARROW_LEFT, Game::EButton::A });
    Game::Inputs::SetAxisAction("vertical", { Game::EButton::SPACE }, { Game::EButton::LEFT_CTRL });
    Game::Inputs::SetAxisAction("forward", { Game::EButton::ARROW_UP, Game::EButton::W }, { Game::EButton::ARROW_DOWN, Game::EButton::S });

    // ADD ENTITIES (for tests)
    for (int i = 0; i < 6; i++)
    {
        Game::Entity* entity = engine.entityManager.CreateEntity("entity_" + std::to_string(i));
        entity->transform.position = Vector3(i * 2.f, 0.f, 0.f);
        entity->transform.scale = Vector3(i * 0.2f + 0.2f, i * 0.2f + 0.2f, i * 0.2f + 0.2f);

        if (i >= 3)
        {
            auto drawable = entity->AddComponent<Game::Drawable>();
            auto& model = drawable->model;
            model.AddMeshesFromFile("game/assets/bp.fbx", "game/assets/bp.jpg", false);
            model.transform.scale = Vector3(0.01f, 0.01f, 0.01f);

            if (i == 3) entity->AddComponent<Bar>();
        }
        else if (i == 1)
        {
            entity->name = "Light";
            entity->AddComponent<Game::LightComponent>();
        }
        else if (i == 2)
        {
            Game::SoundComponent* sc = entity->AddComponent<Game::SoundComponent>();
            sc->sound.SetSound("game/assets/Airport.wav");
            entity->name = "Sound";
        }
        else
        {
            entity->AddComponent<Game::CameraComponent>();
            entity->AddComponent<Game::FPSMovement>();
            entity->name = "Game Camera";
        }
    }

    engine.entityManager.FindLight();
}
