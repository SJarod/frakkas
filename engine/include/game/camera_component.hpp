#pragma once

#include "renderer/lowlevel/camera.hpp"
#include "component.hpp"

namespace Game
{
    static const char cameracomponent[] = "CameraComponent";
    class CameraComponent : public BaseComponent<cameracomponent>
    {
    public:
        void Edit() override;
        void SetOwner(Entity* owner) override;

        Renderer::LowLevel::Camera camera;
    };
}
