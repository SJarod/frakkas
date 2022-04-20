#pragma once

#include "maths.hpp"


namespace Renderer
{
    class Light
    {
    public:
        Light() = default;
        ~Light() = default;

        Vector4 position = {}; // w = 0 for directionnal light
        Vector3 ambient = {0.4f, 0.4f, 0.4f};
        Vector3 diffuse = {1.f, 0.f, 0.f};
        Vector3 specular = {1.f, 1.f, 1.f};

        bool toonShading = false;
        bool fiveTone = false;
        bool outline = false;
    };
}
