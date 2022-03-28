//
// Created by m.mehalin on 22/03/2022.
//

#pragma once

#include "maths.hpp"

typedef void* ImTextureID;

namespace Editor
{
    class Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;

        /**
        * @summary Display the ImGui panel
        */
        Vector2 OnImGuiRender(ImTextureID i_tex);
    };
}