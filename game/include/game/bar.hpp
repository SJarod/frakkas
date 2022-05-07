#pragma once

#include "maths.hpp"
#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(Bar)

        KK_FIELD(Vector3, originPos);
        KK_FIELD(Vector3, destPos) = Vector3(2.f, 2.f, 0.f);

        KK_FIELD(float, speed) = 5.f;
        float lerpTime = 0.f;

        void Start() override;
        void Update() override;
        void OnDestroy() override;

    KK_COMPONENT_END
}

