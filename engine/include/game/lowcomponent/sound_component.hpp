#pragma once

#include "game/component_generator.hpp"
#include "resources/sound.hpp"


namespace Game
{
    KK_COMPONENT(SoundComponent)

    KK_FIELD(Resources::Sound, sound);

    KK_COMPONENT_END
}