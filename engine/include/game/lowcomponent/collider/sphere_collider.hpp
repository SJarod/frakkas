#pragma once

#include "game/component_generator.hpp"
#include "collider.hpp"
#include "renderer/lowlevel/lowrenderer.hpp"

namespace Game
{
    KK_COMPONENT_FROM(SphereCollider, Collider);

        KK_FIELD(float, radius) = 1.f;

        void ApplyEditorUpdate(JPH::BodyInterface* i_bodyInterface) override;

        void DebugDraw(Renderer::LowLevel::LowRenderer& i_renderer) const override;

    private:
        float prevRadius = 1.f;
        /**
         * Create a new sphere shape with input radius.
         * @param i_radius The new radius of the object.
         */
        void ApplyNewRadius(float i_radius);

    KK_COMPONENT_END
}
