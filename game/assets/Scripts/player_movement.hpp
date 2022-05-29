#pragma once

#include "maths.hpp"

#include "game/lowcomponent/collider/sphere_collider.hpp"
#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(PlayerMovement)
        Vector3 startPosition = Vector3::zero;
        float speed = 800.f;

        float leftPower = 0.3f;
        float rightPower = 0.3f;

        SphereCollider* rb = nullptr;

        void OnStart() override;

        void OnUpdate() override;
    KK_COMPONENT_END
}
