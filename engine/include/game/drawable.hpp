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
        void SetOwner(Entity* owner) override;
    };
}