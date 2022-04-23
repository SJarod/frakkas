#pragma once

#include <SDL.h>

#include <list>
#include <initializer_list>
#include <unordered_map>
#include <string>
#include <functional>

#include "maths.hpp"
#include "debug/log.hpp"

#include "utils/enum_array.hpp"
#include "utils/inputs_enum.hpp"

using InputsEvent = std::function<void(const SDL_Event*)>;

namespace Game
{
    struct KeyButton
    {
        EButton key = EButton::NONE;
        EButtonState state = EButtonState::RELEASED;
    };

    struct ButtonAction
    {
        EButtonState state = EButtonState::RELEASED;
        std::list<EButtonState*> buttons {};
    };

    struct AxisAction
    {
        float value = 0.f;
        std::list<EButtonState*> positiveButtons {};
        std::list<EButtonState*> negativeButtons {};
    };

    struct MouseAction
    {
        Vector2 position;
        Vector2 deltaMotion;
    };

    class Inputs
    {
    public:
        Inputs();

        static bool quit;

        /**
        * Listen to SDL inputs Event, and update keys' state
        */
        void PollEvent(const InputsEvent& editorEvent);

        /**
         * Enable or Disable the inputs listening.
         * @param i_listening The new listening state.
         */
        void SetInputsListening(bool i_listening) noexcept;

        /**
         * Create or Modify a button action map.
         * @param i_name Name of action map.
         * @param i_resetKeys Clear the current inputs list (only if map already exists).
         * @param i_inputs The inputs to bind.
         */
        static void SetButtonAction(const std::string& i_name, const std::initializer_list<EButton>& i_inputs, bool i_resetKeys = false);
        /**
         * @param i_name name of the button action map.
         * @return the current state of the button.
         */
        static EButtonState GetButton(const std::string& i_name);

        /**
         * @param i_name name of the button action map.
         * @return true if button pressed, false if not
         */
        static bool IsPressed(const std::string& i_name);
        /**
         * @param i_button the keyboard input key
         * @return true if button pressed, false if not
         */
        static bool IsPressed(EButton i_button);

        /**
         * @param i_name name of the button action map.
         * @return true if button down, false if not
         */
        static bool IsDown(const std::string& i_name);
        /**
         * @param i_button the keyboard input key
         * @return true if button down, false if not
         */
        static bool IsDown(EButton i_button);

        /**
         * @param i_name name of the button action map.
         * @return true if button released, false if not
         */
        static bool IsReleased(const std::string& io_name);
        /**
         * @param i_button the keyboard input key
         * @return true if button released, false if not
         */
        static bool IsReleased(EButton i_button);

        /**
         * Create or Modify an axis action map
         * @param i_name  Name of the action map.
         * @param i_resetPositive Clear the positive inputs list if true (Only when map already exists).
         * @param i_positiveInputs The positive inputs to bind.
         * @param i_resetNegative Clear the negative inputs list if true (Only when map already exists).
         * @param i_negativeInputs The negative inputs to bind.
         */
        static void SetAxisAction(const std::string& i_name, const std::initializer_list<EButton>& i_positiveInputs, const std::initializer_list<EButton>& i_negativeInputs, bool i_resetPositive = false, bool i_resetNegative = false);

        /**
         * @param i_name name of the axis action map
         * @return the current value. 1 if positive inputs pressed/down, -1 if negative inputs pressed/down, else 0.
         */
        static float GetAxis(const std::string& i_name);

        /**
         * @return The current position of the mouse's cursor.
         */
        static Vector2 GetMousePosition();
        /**
         * Get the delta motion. Delta motion is the translation that the cursor did during the last frame.
         * @return The current frame motion of the mouse's cursor.
         */
        static Vector2 GetMouseDelta();

    private:
        static std::unordered_map<SDL_KeyCode, EButton> keys;

        static std::unordered_map<std::string, ButtonAction> buttonActions;
        static std::unordered_map<std::string, AxisAction> axisActions;

        static std::list<EButtonState*> keyPressed;

        static EnumArray < EButton, EButtonState, static_cast<size_t>(EButton::KEY_COUNT)> buttonStates;

        static MouseAction mouse;
        static bool allowListeningInputs;

        /**
         * Change input's state to PRESSED and emplace a pointer into n pressed-key array.
         * @param o_state A reference to the state of the pressed key.
         */
        static void KeyPressed(EButtonState& o_state);
        /**
         * Check on each keys which was pressed the last frame. Set them as DOWN if not RELEASED.
         */
        static void CheckKeyPressed();

        /**
         * @param i_button the input
         * @return a pointer to the static EButtonState of the input
         */
        static EButtonState* GetButtonState(EButton i_button);
        /**
         * Check if the button action map exists and update it if needed.
         * @param i_name the name of the action map.
         * @param i_update update the button action state if true.
         * @return true if button action exists, false if not.
         */
        static bool CheckButtonAction(const std::string& i_name);
        /**
         * Check if the axis action map exists and update it if needed.
         * @param i_name the name of the action map.
         * @param i_update update the axis action value if true.
         * @return true if axis action exists, false if not.
         */
        static bool CheckAxisAction(const std::string& i_name);

        static bool ApplyListeningCheck(bool i_currentValue = true);
    };
}
