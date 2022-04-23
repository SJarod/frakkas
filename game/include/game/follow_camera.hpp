#pragma once

#include "maths.hpp"
#include "game/transform.hpp"

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(FollowCamera)

        Transform* playerTransform = nullptr;
        KK_FIELD(Vector3, offset) = Vector3::zero;

        void Update() override;
    KK_COMPONENT_END
}
