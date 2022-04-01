#pragma once

#include "renderer/lowlevel/camera.hpp"
#include "component.hpp"


namespace Game
{
    class CameraComponent : public Component
    {
    public:
        CameraComponent();

        void Edit() override;

        Renderer::LowLevel::Camera camera;
    };
}