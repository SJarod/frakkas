#pragma once

#include "renderer/skeletal_model.hpp"
#include "game/lowcomponent/drawable.hpp"

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT_FROM(AnimatedDraw, Drawable)

        Renderer::SkeletalModel skmodel;

protected:
    void SetOwner(Entity* owner) override;

    void Update() override;

    void Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light) override;

    KK_COMPONENT_END
}