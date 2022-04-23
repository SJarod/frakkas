#pragma once


class Engine;

namespace Game
{
    class World
    {
    public:
        explicit World(Engine& engine);
        ~World() = default;

    private:
        static const char* defaultScene;
    };
}
