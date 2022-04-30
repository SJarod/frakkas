#include "game/fps_movement.hpp"
#include "game/player_movement.hpp"
#include "game/follow_camera.hpp"
#include "game/bar.hpp"

#include "game/lowcomponent/component_register.hpp"

using namespace Game;

ComponentRegister::ComponentRegister()
    : greedEntity(0, "greed")
{
    greedEntity.AddComponent<FPSMovement>();
    greedEntity.AddComponent<PlayerMovement>();
    greedEntity.AddComponent<FollowCamera>();
    greedEntity.AddComponent<Bar>();
    // TODO:Add new component $
}
