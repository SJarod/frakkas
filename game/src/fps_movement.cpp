
#include "maths.hpp"
#include "engine.hpp"

#include "game/entity.hpp"
#include "game/time_manager.hpp"

#include "game/fps_movement.hpp"

using namespace Game;

KK_COMPONENT_IMPL(FPSMovement)

void FPSMovement::Update()
{
    float frameSpeed = 20.f * Time::GetDeltaTime();

    float forwardVelocity = Inputs::GetAxis("forward") * frameSpeed;
    float strafeVelocity = Inputs::GetAxis("horizontal") * frameSpeed;

    Vector3 pos = owner.get()->transform.position;
    Vector3 rot = owner.get()->transform.rotation;

    pos.x += Maths::Sin(rot.y) * forwardVelocity + Maths::Cos(rot.y) * strafeVelocity;
    pos.z += Maths::Sin(rot.y) * strafeVelocity - Maths::Cos(rot.y) * forwardVelocity;

    rot.y += Inputs::GetMouseDelta().x * 0.005f;
    rot.x -= Inputs::GetMouseDelta().y * 0.005f;

    rot.y = Maths::Modulo(rot.y, Maths::Constants::doublePi);
    rot.x = Maths::Clamp(rot.x, -Maths::Constants::halfPi, Maths::Constants::halfPi);

    owner.get()->transform.position = pos;
    owner.get()->transform.rotation = rot;
}
