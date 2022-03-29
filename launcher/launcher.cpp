//
// Created by f.marcellot on 18/03/2022.
//

#include <iostream>

#include "game/entity.hpp"

int main()
{
    bool running = true;
    int foo = 1;
    while(running)
    {
        /// NEW FRAME

        /// UPDATE

        /// ENDFRAME

        if (foo++ == 3)
            running = false;
    }
    return 0;
}
