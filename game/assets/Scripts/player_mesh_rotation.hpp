#pragma once

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(PlayerMeshRotation);

        void OnUpdate() override;

        Vector2 direction;

    KK_COMPONENT_END
}
