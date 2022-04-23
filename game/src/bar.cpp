#include <iostream>

#include "maths.hpp"
#include "log.hpp"

#include "game/time_manager.hpp"
#include "game/bar.hpp"

KK_COMPONENT_IMPL(Bar)

KK_FIELD_IMPL(Bar, originPos, DataType::FLOAT, 3)
KK_FIELD_IMPL(Bar, destPos, DataType::FLOAT, 3)
KK_FIELD_IMPL(Bar, speed, DataType::FLOAT, 1)

void Game::Bar::Start()
{
    std::cout << "Bar component started" << std::endl;
}

void Bar::Update()
{
    lerpTime = Maths::Sin(Time::GetTime() * speed);
    owner.get()->transform.position = Maths::Lerp(originPos, destPos, lerpTime);
}
