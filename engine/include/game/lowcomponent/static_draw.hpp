#pragma once

#include "renderer/model.hpp"
#include "game/lowcomponent/drawable.hpp"

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT_FROM(StaticDraw, Drawable)

        KK_FIELD(Renderer::Model, model);

    protected:
        void Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform = {}) override;

        void DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform) override;

    KK_COMPONENT_END
}
