#pragma once

#include "maths.hpp"

#include "game/lowcomponent/collider/sphere_collider.hpp"
#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(PlayerMovement)

        SphereCollider* rb = nullptr;
        float speed = 800.f;
        float leftPower = 0.3f;
        float rightPower = 0.3f;

        void OnStart() override;

        void OnUpdate() override;

    KK_COMPONENT_END
}
