#include "player_mesh_rotation.hpp"

KK_COMPONENT_IMPL_BEGIN(PlayerMeshRotation)

KK_COMPONENT_IMPL_END

void PlayerMeshRotation::OnUpdate()
{
    Vector2 curDir = -Vector2(Inputs::GetAxis("horizontal"), Inputs::GetAxis("forward")).Normalize();
    Vector2 curGamepadDir = -Inputs::GetLeftJoystickDirection();

    if (curGamepadDir != Vector2())
        direction = curGamepadDir;
    else if (curDir != Vector2())
        direction = curDir;

    Rotation() = {0.f, Maths::Atan2(direction.x, direction.y), 0.f};
}

