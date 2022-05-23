#include "game/entity.hpp"

#include "player_movement.hpp"
#include "follow_camera.hpp"

using namespace Game;
KK_COMPONENT_IMPL_BEGIN(FollowCamera)
    KK_FIELD_PUSH(FollowCamera, offset, EDataType::FLOAT)
    KK_FIELD_COUNT(3)

    KK_FIELD_PUSH(FollowCamera, lerpFactor, EDataType::FLOAT)
    KK_FIELD_RANGE(0.01f, 1.f)
KK_COMPONENT_IMPL_END

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
