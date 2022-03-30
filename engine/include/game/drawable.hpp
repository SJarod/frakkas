//
// Created by f.marcellot on 28/03/2022.
//

#pragma once  

#include "renderer/model.hpp"

#include "component.hpp"

namespace Game
{
    class Drawable : public Component
    {
    public:
        Drawable();

        void Edit() override;


        Renderer::Model model;

    protected:
        virtual void SetOwner(Entity* owner) override;
    };
}
