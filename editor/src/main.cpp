
#include <iostream>

#include "Editor/editor_render.hpp"

#include "Engine/foo.hpp"

int main() {

    Engine::SayHello();

    bool running = true;
    while(running)
    {
        /// NEW FRAME

        /// UPDATE
        Editor::UpdateAndRender();

        /// ENDFRAME

        running = false;
    }

    return 0;
}
