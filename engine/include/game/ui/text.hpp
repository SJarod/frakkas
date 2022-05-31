#pragma once

#include "game/component_generator.hpp"
#include "ui_object.hpp"

namespace Game
{
    KK_COMPONENT_FROM(Text, UIObject)

        std::string text = "hello world!";
        Vector4 color = {0.f, 0.f, 0.f, 1.f};

        void Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const override;

    KK_COMPONENT_END
}
