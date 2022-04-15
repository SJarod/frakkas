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
        Vector3 ambient = {1.f, 1.f, 1.f};
        Vector3 diffuse = {};
        Vector3 specular = {};

        bool toonShading = false;
        bool fiveTone = false;
        bool outline = false;
    };
}
