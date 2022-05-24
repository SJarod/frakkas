#include <miniaudio.h>

#include "utils/dragdrop_constants.hpp"

#include "resources/resources_manager.hpp"

#include "engine.hpp"

#include "game/lowcomponent/sound_component.hpp"

KK_COMPONENT_IMPL_BEGIN(SoundComponent)
    KK_FIELD_PUSH(SoundComponent, soundPath, EDataType::STRING)
    KK_FIELD_CHANGECALLBACK(OnSoundPathUpdate)
    KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, DropOnSoundComponent)

    KK_FIELD_PUSH(SoundComponent, loop, EDataType::BOOL)

    KK_FIELD_PUSH_BUTTON("Play", OnPlayClicked)
    KK_FIELD_PUSH_BUTTON("Pause", OnPauseClicked)
    KK_FIELD_SAMELINE
    KK_FIELD_PUSH_BUTTON("Stop", OnStopClicked)
    KK_FIELD_SAMELINE

    KK_FIELD_PUSH(SoundComponent, volume, EDataType::FLOAT)
    KK_FIELD_RANGE(0.f, 1.f)
    KK_FIELD_CHANGECALLBACK(OnVolumeUpdate)
KK_COMPONENT_IMPL_END

void SoundComponent::SetSoundPath(const std::filesystem::path& i_path)
{
    soundPath = i_path.string();
    sound = Resources::ResourcesManager::LoadResource<Resources::Sound>(i_path.string());
}


void SoundComponent::Play()
{
    if (!sound->loaded.test()) return;
    ma_sound_set_looping(&sound->soundObject, loop);
    ma_sound_start(&sound->soundObject);
}

void SoundComponent::Stop()
{
    if (!sound->loaded.test()) return;
    ma_sound_stop(&sound->soundObject);
    ma_sound_seek_to_pcm_frame(&sound->soundObject, 0);
}

void SoundComponent::Pause()
{
    if (!sound->loaded.test()) return;
    ma_sound_stop(&sound->soundObject);
}

void SoundComponent::SetVolume()
{
    ma_engine_set_volume(&Engine::soundEngine, volume);
}

void DropOnSoundComponent(unsigned char* io_component, void* i_dropData)
{
    SoundComponent& sc = *reinterpret_cast<SoundComponent*>(io_component);
    std::filesystem::path path = *reinterpret_cast<std::filesystem::path*>(i_dropData);

    if (Utils::FindExtension(path.extension().string(), Utils::SoundExtensions))
        sc.SetSoundPath(path);
}

void OnSoundPathUpdate(unsigned char* io_component)
{
    SoundComponent& sc = *reinterpret_cast<SoundComponent*>(io_component);
    sc.SetSoundPath(sc.soundPath);
}

void OnPlayClicked(unsigned char* io_component)
{
    SoundComponent& sc = *reinterpret_cast<SoundComponent*>(io_component);
    sc.Play();
}

void OnPauseClicked(unsigned char* io_component)
{
    SoundComponent& sc = *reinterpret_cast<SoundComponent*>(io_component);
    sc.Pause();
}

void OnStopClicked(unsigned char* io_component)
{
    SoundComponent& sc = *reinterpret_cast<SoundComponent*>(io_component);
    sc.Stop();
}

void OnVolumeUpdate(unsigned char* io_component)
{
    SoundComponent& sc = *reinterpret_cast<SoundComponent*>(io_component);
    sc.SetVolume();
}