#pragma once

#include "maths.hpp"

#include "game/lowcomponent/collider/sphere_collider.hpp"
#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(PlayerMovement)

        KK_FIELD(Vector3, startPosition) = Vector3::zero;
        KK_FIELD(float, speed) = 800.f;

        KK_FIELD(float, leftPower) = 0.3f;
        KK_FIELD(float, rightPower) = 0.3f;

        SphereCollider* rb = nullptr;

        void OnStart() override;

        void OnUpdate() override;
    KK_COMPONENT_END
}