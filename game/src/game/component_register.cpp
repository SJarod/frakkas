#include "fps_movement.hpp"
#include "player_movement.hpp"
#include "follow_camera.hpp"
#include "game/lowcomponent/static_draw.hpp"
#include "game/lowcomponent/animated_draw.hpp"
#include "game/lowcomponent/camera.hpp"
#include "game/lowcomponent/sound_component.hpp"
#include "game/lowcomponent/drawable.hpp"

#include "game/component_register.hpp"

using namespace Game;

ComponentRegister::ComponentRegister()
    : greedEntity(0, "greed")
{
    // TODO:Add new component $
    greedEntity.AddComponent<StaticDraw>();
    greedEntity.AddComponent<AnimatedDraw>();
    greedEntity.AddComponent<Camera>();
    greedEntity.AddComponent<SoundComponent>();
    greedEntity.AddComponent<FPSMovement>();
    greedEntity.AddComponent<PlayerMovement>();
    greedEntity.AddComponent<FollowCamera>();
    greedEntity.AddComponent<Drawable>();
}
