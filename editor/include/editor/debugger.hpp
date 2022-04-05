#pragma once

namespace Editor
{
    class Debugger
    {
    public:
        Debugger() = default;
        ~Debugger() = default;

        /**
         * @summary Display the imgui panel
         */
        void OnImGuiRender();

    private:
        float prevTime = 0.f;
        float deltaTime = 0.f;
        int FPS = 0;
    };
}
