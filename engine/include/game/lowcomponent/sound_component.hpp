#pragma once

#include "game/component_generator.hpp"
#include "resources/sound.hpp"


namespace Game
{
    KK_COMPONENT(SoundComponent)

    KK_FIELD(Resources::Sound, sound);

    void OnDestroy() override;

    KK_COMPONENT_END
}