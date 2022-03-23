//
// Created by m.mehalin on 16/03/2022.
//

#pragma once

namespace Editor
{
    class MenuBar
    {
    public:
        MenuBar() = default;
        ~MenuBar() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender();
    };
}