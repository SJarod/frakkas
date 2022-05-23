#pragma once

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(RumbleTrigger);

        /// You can remove the functions you don't need.
        void OnStart() override;

        void OnUpdate() override;

        void OnTriggerStay(const Collider* i_ownerCollider, const Collider* i_otherCollider) override;

    KK_COMPONENT_END
}
