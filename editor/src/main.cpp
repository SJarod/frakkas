
#include <iostream>

#include "Engine/entity_manager.hpp"
#include "Editor/editor_render.hpp"
#include "maths.hpp"

int main() {

    Engine::EntityManager::Init();

    bool running = true;
    bool gaming = false;
    while(running)
    {
        /// NEW FRAME
        std::cout << std::endl <<"New Frame and pi = " << Maths::Constants::pi << std::endl;
        

        /// UPDATE
        Editor::UpdateAndRender();


        if (gaming)
            Engine::EntityManager::UpdateAndRender();

        /// ENDFRAME
        running = false;
    }

    return 0;
}
