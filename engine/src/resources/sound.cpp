#include "debug/log.hpp"
#include "engine.hpp"
#include "maths/utils.hpp"

#include "resources/sound.hpp"

void Resources::Sound::SetSound(std::string i_soundPath)
{
    if (i_soundPath.empty())
    {
        Log::Warning("Sound file not found.");
        return;
    }

    soundPath = i_soundPath;
    Init();
}

void Resources::Sound::Init()
{
    if (ma_sound_init_from_file(&Engine::soundEngine, soundPath.c_str(), MA_SOUND_FLAG_DECODE, nullptr, nullptr, &soundObject) != MA_SUCCESS)
        Log::Warning("Fail to load " + soundPath);
    else
        Log::Info("Successfully loaded " + soundPath);
}

void Resources::Sound::Play()
{
    ma_sound_start(&soundObject);
}

void Resources::Sound::Stop()
{
    ma_sound_stop(&soundObject);
    ma_sound_seek_to_pcm_frame(&soundObject, 0);
}

void Resources::Sound::Pause()
{
    ma_sound_stop(&soundObject);
}

void Resources::Sound::SetVolume()
{
    volume = Maths::Clamp(volume, 0, 20);

    ma_engine_set_volume(&Engine::soundEngine, volume);
}