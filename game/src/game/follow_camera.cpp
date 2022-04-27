#include "game/entity.hpp"

#include "game/follow_camera.hpp"

using namespace Game;
KK_COMPONENT_IMPL(FollowCamera)
KK_FIELD_IMPL(FollowCamera, offset, DataType::FLOAT, 3)

void FollowCamera::Update()
{
    //TODO: GetEntity
}
