//
// Created by f.marcellot on 18/03/2022.
//

#include <iostream>

#include "entity.hpp"
#include "Engine/entity_manager.hpp"

int main()
{
    Engine::EntityManager::Init();

    bool running = true;
    int foo = 1;
    while(running)
    {
        /// NEW FRAME

        /// UPDATE
        Engine::EntityManager::UpdateAndRender();

        /// ENDFRAME

        if (foo++ == 3)
            running = false;
    }
    return 0;
}
