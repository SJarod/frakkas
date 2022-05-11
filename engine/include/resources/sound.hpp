#pragma once

#include <miniaudio.h>

#include "maths.hpp"


namespace Resources
{
    class Sound
    {
    public:
        Sound() = default;
        ~Sound() = default;

        ma_sound soundObject;
        std::string soundPath;
        float volume = 1.f;
        bool loop = false;

        /**
         * Set soundPath
         * @param i_soundPath full path to the sound file
         */
        void SetSound(std::string i_soundPath);

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
        /**
         * Init sound from file
         */
        void Init();
    };
}