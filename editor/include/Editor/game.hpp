//
// Created by m.mehalin on 22/03/2022.
//

#pragma once

namespace Editor
{
    class Game
    {
    public:
        Game() = default;
        ~Game() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender();
    };
}