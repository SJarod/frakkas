#include <miniaudio.h>

#include "game/inputs_manager.hpp"

#include "game/lowcomponent/sound_component.hpp"

KK_COMPONENT_IMPL(SoundComponent)
KK_FIELD_IMPL(SoundComponent, sound, EDataType::SOUND)

void SoundComponent::OnDestroy()
{
    if (!Game::Inputs::quit)
        ma_sound_uninit(&sound.soundObject);
}