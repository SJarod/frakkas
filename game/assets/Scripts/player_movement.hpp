#pragma once

#include "maths.hpp"

#include "game/ui/button.hpp"
#include "game/collider/sphere_collider.hpp"
#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(PlayerMovement)

        Button* buttonGame = nullptr;
        Button* buttonUI = nullptr;
        SphereCollider* rb = nullptr;
        float walkSpeed = 400.f;
        float runSpeed = 1000.f;

        float leftPower = 0.3f;
        float rightPower = 0.3f;

        void OnStart() override;

        void OnUpdate() override;

    KK_COMPONENT_END
}
