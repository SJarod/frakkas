#pragma once

#include "renderer/model.hpp"
#include "game/lowcomponent/drawable.hpp"

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT_FROM(StaticDraw, Drawable)

        Renderer::Model model;

    protected:
        void SetOwner(Entity* owner) override;

        void Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light) override;

    KK_COMPONENT_END
}
