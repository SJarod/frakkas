#include "debug/log.hpp"
#include "engine.hpp"

#include "resources/sound.hpp"

using namespace Resources;

Sound::Sound(const std::string& i_name)
{
    soundPath = i_name;
}

Sound::~Sound()
{
    //ma_sound_uninit(&soundObject);
}

void Sound::LoadFromInfo()
{
    resourceType = EResourceType::SOUND;
//    ma_sound_uninit(&soundObject);
    if (ma_sound_init_from_file(&Engine::soundEngine, soundPath.string().c_str(), MA_SOUND_FLAG_DECODE, nullptr, nullptr, &soundObject) != MA_SUCCESS)
        Log::Warning("Failed to init ",soundPath);
    else
        Log::Info("Successfully loaded sound file : ", soundPath);

    ComputeMemorySize();
}

void Sound::ComputeMemorySize()
{
    ram = 0;
    vram = 0;
    float time;
    ma_sound_get_length_in_seconds(&soundObject, &time);
    ram = static_cast<size_t>(ma_calculate_buffer_size_in_frames_from_milliseconds(static_cast<ma_uint32>(time * 1000.f), ma_engine_get_sample_rate(&Engine::soundEngine)));
    // TODO: Improve size computing
}
