#pragma once

#include <memory>
#include <filesystem>
#include <miniaudio.h>

#include "game/component_generator.hpp"


namespace Game
{
    KK_COMPONENT(Sound)

    void SetSoundPath(std::filesystem::path i_path);
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

    /**
     * @brief Unload miniaudio sound resource if 'loaded' is true
     */
    void UnloadSound();

private:
    ma_sound sound;
    bool loaded = false;

    /**
     * @brief Load miniaudio sound resource with current 'soundPath' value.
     */
    void LoadSound();

    KK_COMPONENT_END
}

void DropOnSoundComponent(unsigned char* io_component, void* i_dropData);
void OnSoundPathUpdate(unsigned char* io_component);
void OnPlayClicked(unsigned char* io_component);
void OnPauseClicked(unsigned char* io_component);
void OnStopClicked(unsigned char* io_component);
void OnVolumeUpdate(unsigned char* io_component);