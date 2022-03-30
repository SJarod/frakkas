//
// Created by f.marcellot on 30/03/2022.
//

#pragma once  

#include "renderer/lowlevel/camera.hpp"

#include "component.hpp"

namespace Game{
    class CameraComponent : public Component{
    public:
        CameraComponent();

        void Edit() override;

        Renderer::LowLevel::Camera camera;
    };
}
