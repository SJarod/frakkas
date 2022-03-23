//
// Created by m.mehalin on 22/03/2022.
//

#pragma once

namespace Editor
{
    class Console
    {
    public:
        Console() = default;
        ~Console() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender();
    };
}