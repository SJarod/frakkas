#pragma once

#include "game/component_generator.hpp"
#include "collider.hpp"

namespace Game
{
    KK_COMPONENT_FROM(BoxCollider, Collider);

        KK_FIELD(Vector3, halfExtension) = Vector3(1.f, 1.f, 1.f);

        void ApplyEditorUpdate(JPH::BodyInterface* i_bodyInterface) override;

        void DebugDraw(Renderer::LowLevel::LowRenderer& i_renderer) const override;

    private:
        Vector3 prevExtension = Vector3(1.f, 1.f, 1.f);
        /**
         * Check collider scale and create new scaled shape if needed.
         */
        void UpdateBoxScale();

    KK_COMPONENT_END
}