
#include "game/register_components.hpp"
#include "engine.hpp"

#undef main
int main()
{
    Engine engine;

    Game::RegisterComponents(engine.entityManager);

    engine.RunGame();

    return 0;
}
