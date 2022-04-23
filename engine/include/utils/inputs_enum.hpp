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
        KEY_COUNT
    };

    enum class EButtonState
    {
        RELEASED,
        PRESSED,
        DOWN
    };
}