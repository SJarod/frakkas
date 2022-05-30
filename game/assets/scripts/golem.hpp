#pragma once

#include "game/transform.hpp"
#include "game/lowcomponent/collider/sphere_collider.hpp"
#include "game/lowcomponent/animated_draw.hpp"

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(Golem);

    public:
        SphereCollider* rb = nullptr;

        int damage = 3;

    private:
        Transform* playerTransform = nullptr;
        AnimatedDraw* ad = nullptr;
        Vector3 translation = Vector3::zero;
        Vector3 vecDir = Vector3::zero;
        float speed = 650.f;
        int life = 10;
        bool alive = true;
        float curAttackTime = 0.f;
        float attackTime = 10.f;
        float attackRange = 20.f;
        float distFromPlayer = 0.f;

        void OnStart() override;

        void OnUpdate() override;

        void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        bool IsAlive();

        void Death();

        void Attack();

    KK_COMPONENT_END
}