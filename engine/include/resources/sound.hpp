#pragma once

#include <miniaudio.h>
#include <filesystem>

#include "maths.hpp"

#include "resource.hpp"


namespace Resources
{
    class Sound : public Resource
    {
    public:
        Sound(const std::string& i_name);
        ~Sound();

        ma_sound soundObject;
        std::filesystem::path soundPath;

        void LoadFromInfo() override;

        void ComputeMemorySize() override;
    };
}
