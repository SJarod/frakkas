//
// Created by m.mehalin on 16/03/2022.
//

#pragma once

namespace Editor
{
    class Hierarchy
    {
    public:
        Hierarchy() = default;
        ~Hierarchy() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender();
    };
}
