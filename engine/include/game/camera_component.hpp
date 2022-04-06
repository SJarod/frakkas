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
        void SetOwner(Entity* owner) override;

        Renderer::LowLevel::Camera camera;
    };
}
