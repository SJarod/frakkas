// REPLACE '$name' BY COMPONENT NAME
#pragma once

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT($name);

        void Start() override;

        void Update() override;

    KK_COMPONENT_END
}
