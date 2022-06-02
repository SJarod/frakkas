#pragma once

#include "game/world.hpp"

namespace Game
{
    struct FrakkarenaWorldData : public WorldData
    {
        int life = 100;

        int damageLevel = 0;
        int lifeLevel = 0;
        int speedLevel = 0;
        int damageSpeedLevel = 0;

        int score = 0;
        int level = 0;
    };
}
