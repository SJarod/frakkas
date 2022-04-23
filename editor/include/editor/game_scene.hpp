#pragma once

#include "renderer/lowlevel/lowrenderer.hpp"

namespace Editor
{
    class GameScene
    {
    public:
        GameScene() = default;
        ~GameScene() = default;

        /**
         * Tell the editor if user is playing or not.
         */
        bool focusOnGaming = false;
        /**
         * Position where the cursor is locked when user begin playing.
         */
        Vector2 mouseLockPosition;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender(const Renderer::LowLevel::Framebuffer& i_fbo, bool i_gaming);
    };
}