
#include "engine.hpp"
#include "game/entity.hpp"
#include "game/time_manager.hpp"
#include "player_movement.hpp"

using namespace Game;

KK_COMPONENT_IMPL(PlayerMovement)
KK_FIELD_VECTOR_IMPL(PlayerMovement, startPosition, EDataType::FLOAT, 3)
KK_FIELD_IMPL(PlayerMovement, speed, EDataType::FLOAT)

void PlayerMovement::OnUpdate()
{
    float xTranslation = Inputs::GetAxis("horizontal");
    float zTranslation = Inputs::GetAxis("forward");

    Vector2 gamepadDir = Inputs::GetLeftJoystickDirection();

    xTranslation = Maths::Clamp( xTranslation + gamepadDir.x, -1.f, 1.f) * speed * Time::GetDeltaTime();
    zTranslation = Maths::Clamp( zTranslation + gamepadDir.y, -1.f, 1.f) * speed * Time::GetDeltaTime();

    Vector3 translation = Vector3(xTranslation, 0.f, -zTranslation);
    Position() = Position().get() + translation;
}