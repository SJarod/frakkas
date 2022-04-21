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

        std::string soundPath;
        int volume = 1;

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
        ma_sound soundObject;

        /**
         * Init sound from file
         */
        void Init();
    };
}