#pragma once

#include "renderer/model.hpp"
#include "component.hpp"


namespace Game
{
    static const char drawable[] = "Drawable";
    class Drawable : public BaseComponent<drawable>
    {
    public:
        void Edit() override;

        Renderer::Model model;

    protected:
        void SetOwner(Entity* owner) override;
    };
}