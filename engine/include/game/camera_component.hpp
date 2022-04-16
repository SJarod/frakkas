#pragma once

#include "maths.hpp"

#include "renderer/lowlevel/camera.hpp"
#include "component.hpp"

namespace Game
{
    KK_COMPONENT(CameraComponent)

        KK_FIELD(Renderer::LowLevel::Camera, camera);

        void SetOwner(Entity* owner) override;

    KK_COMPONENT_END
}


