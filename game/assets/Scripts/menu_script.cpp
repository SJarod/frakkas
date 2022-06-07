#include "game/ui/button.hpp"
#include "game/ui/image_button.hpp"

#include "menu_script.hpp"

KK_COMPONENT_IMPL_BEGIN(MenuScript)

KK_COMPONENT_IMPL_END

void MenuScript::OnStart()
{
    World::SetInputsMode(World::InputsMode_UI);
    Log::Info("Start");

    auto playButton = owner.get()->GetComponent<ImageButton>();
    playButton->AddClickEvent([&]()
    {
        World::SetInputsMode(World::InputsMode_Game);
        World::LoadScene("game/assets/Scenes/exemple_scene.kk", "game/assets/Textures/gold.jpg", "Tips : Hello world !", 10.f);
    });
    auto quitButton = owner.get()->GetComponent<Button>();
    quitButton->AddClickEvent([&]()
    {
        Inputs::quit = true;
    });
}

void MenuScript::OnUpdate()
{}