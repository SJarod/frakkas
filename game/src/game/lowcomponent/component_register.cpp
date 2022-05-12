#include "game/lowcomponent/static_draw.hpp"
#include "game/lowcomponent/animated_draw.hpp"
#include "game/lowcomponent/camera.hpp"

#include "game/lowcomponent/sound_component.hpp"
#include "game/fps_movement.hpp"
#include "game/player_movement.hpp"
#include "game/follow_camera.hpp"

#include "game/lowcomponent/component_register.hpp"

using namespace Game;

ComponentRegister::ComponentRegister()
    : greedEntity(0, "greed")
{
    greedEntity.AddComponent<StaticDraw>();
    greedEntity.AddComponent<AnimatedDraw>();
    //greedEntity.AddComponent<Camera>();

    greedEntity.AddComponent<SoundComponent>();
    greedEntity.AddComponent<FPSMovement>();
    greedEntity.AddComponent<PlayerMovement>();
    greedEntity.AddComponent<FollowCamera>();

    // TODO:Add new component $
}
