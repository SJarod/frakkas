#pragma once

#include "component_generator.hpp"
#include "renderer/light.hpp"


namespace Game
{
    KK_COMPONENT(LightComponent)

        KK_FIELD(Renderer::Light, light);

    KK_COMPONENT_END
}