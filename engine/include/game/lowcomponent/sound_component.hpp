#pragma once

#include <memory>
#include <filesystem>

#include "game/component_generator.hpp"
#include "resources/sound.hpp"


namespace Game
{
    KK_COMPONENT(SoundComponent)

    void SetSoundPath(const std::filesystem::path& i_path);
    std::string soundPath = "none";

    float volume = 1.f;
    bool loop = false;

    /**
     * Play a sound
     */
    void Play();

    /**
     * Stop a sound
     */
    void Stop();

    /**
     * Pause a sound
     */
    void Pause();

    /**
     * Set the volume of the sound playing
     */
    void SetVolume();

private:
    std::shared_ptr<Resources::Sound> sound;

    KK_COMPONENT_END
}

void DropOnSoundComponent(unsigned char* io_component, void* i_dropData);
void OnSoundPathUpdate(unsigned char* io_component);
void OnPlayClicked(unsigned char* io_component);
void OnPauseClicked(unsigned char* io_component);
void OnStopClicked(unsigned char* io_component);
void OnVolumeUpdate(unsigned char* io_component);