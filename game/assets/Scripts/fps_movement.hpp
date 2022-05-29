#pragma once

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(FPSMovement);

        void OnUpdate() override;

    KK_COMPONENT_END
}
