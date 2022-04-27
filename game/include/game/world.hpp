#pragma once

#include "game/lowcomponent/component_register.hpp"

class Engine;

namespace Game
{
    class World
    {
    public:
        explicit World(Engine& engine);
        ~World() = default;

    private:

        ComponentRegister componentRegister;

        static const char* defaultScene;
    };
}
