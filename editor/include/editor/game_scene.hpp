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
        void OnImGuiRender(ImTextureID i_tex);
    };
}