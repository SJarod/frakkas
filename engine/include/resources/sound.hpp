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
        ~Sound() = default;

        ma_sound soundObject;
        std::filesystem::path soundPath;

        bool DependenciesReady() override { return true; }
        bool CPULoad() override;
        bool GPULoad() override { return true; }
        bool CPUUnload() override;
        bool GPUUnload() override { return true; }
    };
}
