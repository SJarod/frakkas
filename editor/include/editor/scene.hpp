//
// Created by m.mehalin on 22/03/2022.
//

#pragma once

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
        void OnImGuiRender();
    };
}