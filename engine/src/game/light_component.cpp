#include <imgui.h>

#include "helpers/editor_helpers.hpp"

#include "game/light_component.hpp"


using namespace Game;

void LightComponent::Edit()
{
    if (Helpers::ComponentBeginEdit(this))
    {
        Helpers::EditLight(light);
        Helpers::ComponentEndEdit();
    }
}