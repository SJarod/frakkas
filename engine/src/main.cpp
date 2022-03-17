
#include <iostream>

#include "Editor/editor_render.hpp"

#include "Engine/foo.hpp"

#define PROCESS_EDITOR

int main() {
    SayHello();

    bool running = true;
    while(running)
    {
        /// NEW FRAME

#ifdef PROCESS_EDITOR
        Editor::UpdateAndRender();
#else
        //TODO: EntityManager::UpdateAndRender();
#endif

        /// ENDFRAME

        getchar();
        running = false;
    }

    return 0;
}
