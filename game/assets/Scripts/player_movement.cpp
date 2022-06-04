#include "engine.hpp"
#include "game/entity.hpp"
#include "game/time_manager.hpp"

#include "player_movement.hpp"
#include "game/ui/button.hpp"

using namespace Game;

KK_COMPONENT_IMPL_BEGIN(PlayerMovement)

KK_FIELD_PUSH(PlayerMovement, walkSpeed, EDataType::FLOAT)
KK_FIELD_PUSH(PlayerMovement, runSpeed, EDataType::FLOAT)

KK_FIELD_PUSH(PlayerMovement, leftPower, EDataType::FLOAT)
KK_FIELD_RANGE(0.f, 1.f)

KK_FIELD_PUSH(PlayerMovement, rightPower, EDataType::FLOAT)
KK_FIELD_RANGE(0.f, 1.f)

KK_COMPONENT_IMPL_END

void PlayerMovement::OnStart()
{
	rb = owner.get()->GetComponent<SphereCollider>();

	buttonGame = owner.get()->AddComponent<Button>();
	buttonGame->text = "game";
	buttonGame->position = { 10.f, 5.f };
	buttonGame->scale = { 100.f, 20.f };
	buttonGame->AddClickEvent([&]()
		{
			if (World::GetInputsMode() & World::InputsMode_Game)
				World::SetInputsMode(World::InputsMode_Game);
			else
				World::SetInputsMode(World::InputsMode_Game | World::InputsMode_UI);
		});
	buttonGame->enabled = false;

	buttonUI = owner.get()->AddComponent<Button>();
	buttonUI->text = "ui";
	buttonUI->scale = { 100.f, 20.f };
	buttonUI->position = { 10.f, 15.f };
	buttonUI->AddClickEvent([&]()
		{
			World::SetInputsMode(World::InputsMode_UI);
		});
	buttonUI->enabled = false;

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

	float speed = walkSpeed;
	if (Inputs::IsDown(EButton::LEFT_SHIFT))
		speed = runSpeed;

	xTranslation = Maths::Clamp(xTranslation + gamepadDir.x, -1.f, 1.f);
	zTranslation = Maths::Clamp(zTranslation + gamepadDir.y, -1.f, 1.f);

	Vector3 dir = Vector3(xTranslation, 0.f, -zTranslation).Normalize() * speed * Time::GetDeltaTime();
	rb->velocity = dir + Vector3(0.f, rb->velocity.y, 0.f);

	if (Inputs::IsPressed(EButton::P))
	{
		World::SetInputsMode(World::InputsMode_Game | World::InputsMode_UI);
		buttonGame->enabled = true;
		buttonUI->enabled = true;
	}

}