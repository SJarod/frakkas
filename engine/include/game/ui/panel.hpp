#pragma once

#include "game/ui/ui_object.hpp"
#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT_FROM(Panel, UIObject)

        void Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const override;

    KK_COMPONENT_END
}