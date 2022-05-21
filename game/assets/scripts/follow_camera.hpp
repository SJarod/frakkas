#pragma once

#include "maths.hpp"
#include "game/transform.hpp"

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(FollowCamera)

        Transform* playerTransform = nullptr;
        KK_FIELD(Vector3, offset) = Vector3::zero;
        KK_FIELD(float, lerpFactor) = 0.08f;

        void OnEnable() override;

        void OnUpdate() override;
    KK_COMPONENT_END
}
