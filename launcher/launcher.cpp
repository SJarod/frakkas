//
// Created by f.marcellot on 18/03/2022.
//

#include <iostream>

#include "entity.hpp"
#include "engine/entity_manager.hpp"

int main()
{
    Engine::EntityManager entityManager;

    bool running = true;
    int foo = 1;
    while(running)
    {
        /// NEW FRAME

        /// UPDATE
        entityManager.Update();

        /// ENDFRAME

        if (foo++ == 3)
            running = false;
    }
    return 0;
}
