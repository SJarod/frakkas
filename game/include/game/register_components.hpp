#pragma once

//TODO: Add include $i
#include "menu_script.hpp"
#include "player_mesh_rotation.hpp"
#include "fps_movement.hpp"
#include "player_movement.hpp"
#include "follow_camera.hpp"
#include "player.hpp"
#include "golem.hpp"
#include "skeleton.hpp"
#include "spawner.hpp"
#include "game/ui/panel.hpp"
#include "game/ui/image_panel.hpp"
#include "game/ui/image_button.hpp"
#include "game/ui/text.hpp"
#include "game/ui/button.hpp"
#include "game/lowcomponent/static_draw.hpp"
#include "game/lowcomponent/animated_draw.hpp"
#include "game/lowcomponent/camera.hpp"
#include "game/lowcomponent/sound_component.hpp"
#include "game/lowcomponent/drawable.hpp"

#include "game/entity.hpp"
#include "game/entity_manager.hpp"

namespace Game
{
    void RegisterComponents(EntityManager& io_entityManager)
    {
        Entity greedEntity(0, "greed");

        // TODO:Add new component $c
        greedEntity.AddComponent<Panel>();
        greedEntity.AddComponent<Button>();
        greedEntity.AddComponent<ImageButton>();
        greedEntity.AddComponent<Text>();
        greedEntity.AddComponent<ImagePanel>();
		greedEntity.AddComponent<MenuScript>();
        greedEntity.AddComponent<PlayerMeshRotation>();
        greedEntity.AddComponent<StaticDraw>();
        greedEntity.AddComponent<AnimatedDraw>();
        greedEntity.AddComponent<Camera>();
        greedEntity.AddComponent<Player>();
        greedEntity.AddComponent<Golem>();
        greedEntity.AddComponent<Skeleton>();
        greedEntity.AddComponent<Spawner>();
        greedEntity.AddComponent<SoundComponent>();
        greedEntity.AddComponent<FPSMovement>();
        greedEntity.AddComponent<PlayerMovement>();
        greedEntity.AddComponent<FollowCamera>();
        greedEntity.AddComponent<Drawable>();
    }
}
