#pragma once

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(NewComponent);

        void Start() override;

        void Update() override;

    KK_COMPONENT_END
}
