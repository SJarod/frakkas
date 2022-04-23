#pragma once

#include "engine.hpp"

namespace Editor
{
    /**
     * @brief Editor::Debugger is used to provide divers information or utils.
     * In other word, the debugger is the "tote everything"
     * where you can use ImGui without make a mess of the editor.
     */
    class Debugger
    {
    public:
        Debugger() = default;
        ~Debugger() = default;

        /**
         * @summary Display the imgui panel
         */
        void OnImGuiRender(Game::EntityManager& io_entityManager, bool& o_gaming, bool& o_reloadScene);

    private:
        float prevTime = 0.f;
        float deltaTime = 0.f;
        int FPS = 0;

        const float timeBeforeUpdate = 0.2f;
    };
}
