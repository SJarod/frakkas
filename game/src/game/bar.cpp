#include <iostream>

#include "maths.hpp"

#include "game/bar.hpp"

KK_COMPONENT_IMPL(Bar)

KK_FIELD_RANGE_IMPL(Bar, speed, EDataType::FLOAT, 1, 0.1f, 50.f)
KK_FIELD_IMPL(Bar, originPos, EDataType::FLOAT, 3)
KK_FIELD_IMPL(Bar, destPos, EDataType::FLOAT, 3)

void Game::Bar::Start()
{
    Log::Info("Bar component started");
}

void Bar::Update()
{
    lerpTime = Maths::Sin(Time::GetTime() * speed * Time::GetDeltaTime());
    owner.get()->transform.position = Maths::Lerp(originPos, destPos, lerpTime);
}

void Bar::OnDestroy()
{
    Log::Info("Bar component destroyed");
}