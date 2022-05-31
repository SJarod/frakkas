#pragma once

#include <imgui.h>
#include <memory>
#include <vector>

#include "maths.hpp"
#include "game/ui/ui_object.hpp"

namespace UI
{
    class Canvas
    {
    public:
        Canvas();
        ~Canvas() = default;


        ImGuiWindowFlags windowSettings;

        /**
         * @brief Set UIObject ID and add it to rendering loop
         * @param io_uiObject The new built-UIObject to add.
         */
        void AddUIObject(Game::UIObject* io_uiObject);

        /**
         * @brief Check if UIObject is in objects list and remove it.
         * @param i_uiObject The UIObject to remove from rendering loop.
         */
        void RemoveUIObject(const Game::UIObject* i_uiObject);

        /**
         * @brief Remove all the UIObjects from rendering loop.
         */
        void Clear();

        /**
         * @brief Begin an ImGui window and render ui objects
         */
        void BeginAndRender() const;

        /**
         * @brief Render ui objects, an imgui window has to be opened !
         */
        void Render(const Vector2& i_offset = Vector2()) const;

    private:
        std::vector<Game::UIObject*> objects;

        Game::UIObjectIdentifier maxID = 1;
    };
}
