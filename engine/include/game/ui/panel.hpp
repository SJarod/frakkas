#pragma once

#include "game/ui/ui_object.hpp"
#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT_FROM(Panel, UIObject)

        Vector4 color = {0.5f, 0.5f, 0.5f, 0.5f};

        void Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const override;

    KK_COMPONENT_END
}