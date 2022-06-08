#pragma once

#include "sound.hpp"

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(PortalManager);

        float activationTime;
        float closeTime;

        Sound* portalSound = nullptr;
        Sound* teleportationSound = nullptr;

        void OnStart() override;


    KK_COMPONENT_END
}
