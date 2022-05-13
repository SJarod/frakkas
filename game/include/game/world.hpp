#pragma once

#include "component_register.hpp"

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
