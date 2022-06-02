#include <miniaudio.h>

#include "utils/dragdrop_constants.hpp"

#include "resources/resources_manager.hpp"

#include "engine.hpp"

#include "game/lowcomponent/sound.hpp"

KK_COMPONENT_IMPL_BEGIN(Sound)
    KK_FIELD_PUSH(Sound, soundPath, EDataType::STRING)
    KK_FIELD_CHANGECALLBACK(OnSoundPathUpdate)
    KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, DropOnSoundComponent)

    KK_FIELD_PUSH(Sound, loop, EDataType::BOOL)

    KK_FIELD_PUSH_BUTTON("Play", OnPlayClicked)
    KK_FIELD_PUSH_BUTTON("Pause", OnPauseClicked)
    KK_FIELD_SAMELINE
    KK_FIELD_PUSH_BUTTON("Stop", OnStopClicked)
    KK_FIELD_SAMELINE

    KK_FIELD_PUSH(Sound, volume, EDataType::FLOAT)
    KK_FIELD_RANGE(0.f, 1.f)
    KK_FIELD_CHANGECALLBACK(OnVolumeUpdate)
KK_COMPONENT_IMPL_END

void Sound::SetSoundPath(std::filesystem::path i_path)
{
    UnloadSound();
    soundPath = i_path.make_preferred().string();
    LoadSound();
}

void Sound::LoadSound()
{
    if (ma_sound_init_from_file(&Engine::soundEngine, soundPath.c_str(), MA_SOUND_FLAG_DECODE, nullptr, nullptr, &sound) != MA_SUCCESS)
    {
        Log::Warning("Failed to init sound : ", soundPath);
        loaded = false;
    }
    else
        loaded = true;
}

void Sound::UnloadSound()
{
    if (loaded)
        ma_sound_uninit(&sound);
}

void Sound::Play()
{
    if (!loaded) return;
    ma_sound_set_looping(&sound, loop);
    ma_sound_start(&sound);
}

void Sound::Stop()
{
    if (!loaded) return;
    ma_sound_stop(&sound);
    ma_sound_seek_to_pcm_frame(&sound, 0);
}

void Sound::Pause()
{
    if (!loaded) return;
    ma_sound_stop(&sound);
}

void Sound::SetVolume()
{
    ma_engine_set_volume(&Engine::soundEngine, volume);
}

void DropOnSoundComponent(unsigned char* io_component, void* i_dropData)
{
    Sound& sc = *reinterpret_cast<Sound*>(io_component);
    std::filesystem::path path = *reinterpret_cast<std::filesystem::path*>(i_dropData);

    if (Utils::FindExtension(path.extension().string(), Utils::SoundExtensions))
        sc.SetSoundPath(path);
}

void OnSoundPathUpdate(unsigned char* io_component)
{
    Sound& sc = *reinterpret_cast<Sound*>(io_component);
    sc.SetSoundPath(sc.soundPath);
}

void OnPlayClicked(unsigned char* io_component)
{
    Sound& sc = *reinterpret_cast<Sound*>(io_component);
    sc.Play();
}

void OnPauseClicked(unsigned char* io_component)
{
    Sound& sc = *reinterpret_cast<Sound*>(io_component);
    sc.Pause();
}

void OnStopClicked(unsigned char* io_component)
{
    Sound& sc = *reinterpret_cast<Sound*>(io_component);
    sc.Stop();
}

void OnVolumeUpdate(unsigned char* io_component)
{
    Sound& sc = *reinterpret_cast<Sound*>(io_component);
    sc.SetVolume();
}