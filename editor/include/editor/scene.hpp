//
// Created by m.mehalin on 22/03/2022.
//

#pragma once

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
        void OnImGuiRender(ImTextureID i_tex);
    };
}