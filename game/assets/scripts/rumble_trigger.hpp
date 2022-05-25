#pragma once

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(RumbleTrigger);

        float rumblePower = 0.4f;
        Vector3 startPosition;
        bool rumbling = false;


        /// You can remove the functions you don't need.
        void OnStart() override;

        void OnUpdate() override;

        void OnCollisionStay(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;
        void OnCollisionEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;
        void OnCollisionExit(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        void OnTriggerStay(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        void OnTriggerExit(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

    KK_COMPONENT_END
}
