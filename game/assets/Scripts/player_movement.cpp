#include "engine.hpp"
#include "game/entity.hpp"
#include "game/time_manager.hpp"

#include "player_movement.hpp"

using namespace Game;

KK_COMPONENT_IMPL_BEGIN(PlayerMovement)

    KK_FIELD_PUSH(PlayerMovement, speed, EDataType::FLOAT)

    KK_FIELD_PUSH(PlayerMovement, leftPower, EDataType::FLOAT)
    KK_FIELD_RANGE(0.f, 1.f)

    KK_FIELD_PUSH(PlayerMovement, rightPower, EDataType::FLOAT)
    KK_FIELD_RANGE(0.f, 1.f)

KK_COMPONENT_IMPL_END

void PlayerMovement::OnStart()
{
    rb = owner.get()->GetComponent<SphereCollider>();
}

void PlayerMovement::OnUpdate()
{
    float xTranslation = Inputs::GetAxis("horizontal");
    float zTranslation = Inputs::GetAxis("forward");

    Vector2 gamepadDir = Inputs::GetLeftJoystickDirection();

    if (Inputs::IsDown(EButton::GAMEPAD_A))
        Inputs::RumbleGamepadPro(Time::GetDeltaTime(), leftPower, rightPower);
    else if (Inputs::IsDown(EButton::GAMEPAD_X))
        Inputs::RumbleGamepadPro(Time::GetDeltaTime(), leftPower, 0.f);
    else if (Inputs::IsDown(EButton::GAMEPAD_B))
        Inputs::RumbleGamepadPro(Time::GetDeltaTime(), 0.0f, rightPower);

    xTranslation = Maths::Clamp( xTranslation + gamepadDir.x, -1.f, 1.f) * speed * Time::GetDeltaTime();
    zTranslation = Maths::Clamp( zTranslation + gamepadDir.y, -1.f, 1.f) * speed * Time::GetDeltaTime();

    Vector3 translation = Vector3(xTranslation, rb->velocity.y, -zTranslation);
    rb->velocity = translation;
}
