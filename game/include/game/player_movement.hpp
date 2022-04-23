#pragma once

#include "maths.hpp"
#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(PlayerMovement)

        KK_FIELD(Vector3, startPosition) = Vector3::zero;
        KK_FIELD(float, speed) = 3.f;

        void Start() override;

        void Update() override;
    KK_COMPONENT_END
}