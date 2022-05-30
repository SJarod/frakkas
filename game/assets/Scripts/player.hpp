#pragma once

#include "game/transform.hpp"
#include "game/lowcomponent/collider/sphere_collider.hpp"
#include "game/lowcomponent/animated_draw.hpp"

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(Player);

    public:
        SphereCollider* rb = nullptr;
        int damage = 3;

    private:
        Transform* playerTransform = nullptr;
        AnimatedDraw* ad = nullptr;
        int life = 10;

        void OnStart() override;

        void OnUpdate() override;

        void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        bool IsAlive();

        void Death();

        void Attack();

    KK_COMPONENT_END
}
