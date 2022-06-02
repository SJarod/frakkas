
#include "game/register_components.hpp"
#include "game/world_data.hpp"
#include "engine.hpp"

#undef main
int main()
{
    Engine engine;

    Game::RegisterComponents(engine.entityManager);

    Game::World::SetWorldData(std::make_unique<Game::FrakkarenaWorldData>());

    engine.RunGame();

    return 0;
}
