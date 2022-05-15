#include "game/world.hpp"

#include "engine.hpp"

#undef main
int main()
{
    Engine engine;

    Game::World world(engine);

    engine.RunGame();

    return 0;
}
