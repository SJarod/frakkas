#include "game/entity.hpp"

#include "player_movement.hpp"
#include "follow_camera.hpp"

using namespace Game;
KK_COMPONENT_IMPL(FollowCamera)
KK_FIELD_VECTOR_IMPL(FollowCamera, offset, EDataType::FLOAT, 3)
KK_FIELD_RANGE_IMPL(FollowCamera, lerpFactor, EDataType::FLOAT, 1, 0.01f, 1.f)

void FollowCamera::OnEnable()
{
    playerTransform = &GetEntityContainer().FindEntityWithComponent<PlayerMovement>()->transform;
    Position() =playerTransform->position.get() + offset;
}

void FollowCamera::OnUpdate()
{
    if (playerTransform)
    {
        Vector3 targetPosition = offset + playerTransform->position.get();
        Position() = Maths::Lerp(Position().get(), targetPosition, lerpFactor);
    }
}
