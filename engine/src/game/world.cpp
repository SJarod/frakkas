#include "engine.hpp"

#include "renderer/graph.hpp"

#include "game/entity.hpp"
#include "game/entity_manager.hpp"

#include "game/world.hpp"

using namespace Game;

InputsMode World::inputsMode;
Engine* World::engine;

World::World(Engine& i_engine)
{
    engine = &i_engine;

	// ADD DEFAULT AXIS AND BUTTON
	Inputs::SetAxisAction("horizontal",
                                { EButton::ARROW_RIGHT, EButton::D, EButton::GAMEPAD_DPAD_RIGHT },
                                { EButton::ARROW_LEFT, EButton::A, EButton::GAMEPAD_DPAD_LEFT });

	Inputs::SetAxisAction("vertical",
                          { EButton::SPACE }, { EButton::LEFT_CTRL });

    Inputs::SetAxisAction("verticalDebug",
                          { EButton::SPACE, EButton::E }, { EButton::LEFT_CTRL, EButton::Q });

	Inputs::SetAxisAction("forward",
                          { EButton::ARROW_UP, EButton::W, EButton::GAMEPAD_DPAD_UP },
                          { EButton::ARROW_DOWN, EButton::S, EButton::GAMEPAD_DPAD_DOWN });

    LoadScene(defaultScenePath);
}

void World::SetInputsMode(InputsMode i_flag)
{
    if (engine->GetRunMode() & Utils::UpdateFlag_Editing)
        return;

    // Reset everything
    engine->SetUINavigation(false);
    Engine::SetCursorGameMode(true);
    engine->DisableInputs();

    // Set according to flag
    if (i_flag & InputsMode_UI)
    {
        engine->SetUINavigation(true);
        Engine::SetCursorGameMode(false);
    }

    if (i_flag & InputsMode_Game)
    {
        engine->EnableInputs();
    }

    inputsMode = i_flag;
}

InputsMode World::GetInputsMode()
{
    return inputsMode;
}

void World::LoadScene(const std::string& i_scenePath)
{
    engine->graph->SetLoadingParameters(true, i_scenePath);
}

void World::ReloadScene()
{
    engine->graph->SetLoadingParameters(false);
}
