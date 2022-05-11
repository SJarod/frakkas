#pragma once

#include "renderer/skeletal_model.hpp"
#include "game/lowcomponent/drawable.hpp"

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT_FROM(AnimatedDraw, Drawable)

        KK_FIELD(Renderer::SkeletalModel, skmodel);

protected:
    void OnUpdate() override;

    void Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform = {}) override;

    KK_COMPONENT_END
}