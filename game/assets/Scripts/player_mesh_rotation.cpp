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

    Quaternion thisQ = Quaternion::QuatFromEuler(Rotation().get());
    Quaternion targetQ = Quaternion::QuatFromAxisAngle({ 0.f, 1.f, 0.f }, Maths::Atan2(direction.x, direction.y));
    Vector3 rot = Quaternion::Slerp(thisQ, targetQ, Time::GetDeltaTime() * 10.f).QuatToEuler();

    Rotation() = rot;
}

