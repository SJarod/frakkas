//
// Created by m.mehalin on 22/03/2022.
//

#pragma once

namespace Editor
{
    class Inspector
    {
    public:
        Inspector() = default;
        ~Inspector() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender();
    };
}