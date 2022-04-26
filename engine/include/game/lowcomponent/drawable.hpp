#pragma once

#include "renderer/model.hpp"

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(Drawable)

        Renderer::Model model;

    protected:
        void SetOwner(Entity* owner) override;

    KK_COMPONENT_END
}
