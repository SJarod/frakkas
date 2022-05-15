#pragma once

namespace Game
{
    enum class EButton
    {
        NONE,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        LEFT_SHIFT,
        LEFT_CTRL,
        SPACE,
        ARROW_LEFT,
        ARROW_UP,
        ARROW_RIGHT,
        ARROW_DOWN,
        ESCAPE,

        MOUSE_LEFT,
        MOUSE_MIDDLE,
        MOUSE_RIGHT,

        GAMEPAD_A,
        GAMEPAD_B,
        GAMEPAD_X,
        GAMEPAD_Y,
        GAMEPAD_BACK,
        GAMEPAD_GUIDE,
        GAMEPAD_START,
        GAMEPAD_JOYSTICK_LEFT,
        GAMEPAD_JOYSTICK_RIGHT,
        GAMEPAD_SHOULDER_LEFT,
        GAMEPAD_SHOULDER_RIGHT,
        GAMEPAD_DPAD_UP,
        GAMEPAD_DPAD_DOWN,
        GAMEPAD_DPAD_LEFT,
        GAMEPAD_DPAD_RIGHT,

        LEFT_JOYSTICK_LEFT,
        LEFT_JOYSTICK_RIGHT,
        LEFT_JOYSTICK_UP,
        LEFT_JOYSTICK_DOWN,
        RIGHT_JOYSTICK_LEFT,
        RIGHT_JOYSTICK_RIGHT,
        RIGHT_JOYSTICK_UP,
        RIGHT_JOYSTICK_DOWN,

        BUTTON_COUNT
    };

    enum class EButtonState
    {
        RELEASED,
        PRESSED,
        DOWN
    };
}