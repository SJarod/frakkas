#pragma once

#include "renderer/model.hpp"

#include "component.hpp"


namespace Game
{
    KK_COMPONENT(Drawable)

        Renderer::Model model;

    protected:
        void SetOwner(Entity* owner) override;

    KK_COMPONENT_END
}
