//
// Created by m.mehalin on 22/03/2022.
//

#pragma once

namespace Editor
{
    class GameScene
    {
    public:
        GameScene() = default;
        ~GameScene() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender();
    };
}