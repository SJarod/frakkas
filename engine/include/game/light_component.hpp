#pragma once

#include "component.hpp"
#include "renderer/light.hpp"


namespace Game
{
    static const char lightcomponent[] = "LightComponent";
    class LightComponent : public BaseComponent<lightcomponent>
    {
    public:
        Renderer::Light light;

        void Edit() override;
    };
};