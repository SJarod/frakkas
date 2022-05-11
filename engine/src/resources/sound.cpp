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
    else
        ma_sound_uninit(&soundObject);

    soundPath = i_soundPath;
    Init();
}

void Resources::Sound::Init()
{
    if (ma_sound_init_from_file(&Engine::soundEngine, soundPath.c_str(), MA_SOUND_FLAG_DECODE, nullptr, nullptr, &soundObject) != MA_SUCCESS)
        Log::Warning("Fail to init " + soundPath);
    else
        Log::Info("Successfully init " + soundPath);
}

void Resources::Sound::Play()
{
    ma_sound_set_looping(&soundObject, loop);

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
    ma_engine_set_volume(&Engine::soundEngine, volume);
}