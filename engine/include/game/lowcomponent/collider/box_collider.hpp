#pragma once

#include "game/component_generator.hpp"
#include "collider.hpp"

namespace Game
{
    KK_COMPONENT_FROM(BoxCollider, Collider);

        void ApplyEditorUpdate(JPH::BodyInterface* i_bodyInterface) override;

        void DebugDraw(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform) const override;

    private:
        Vector3 prevExtension;

        /**
         * Check collider scale and create new scaled shape if needed.
         */
        void UpdateBoxScale();

    KK_COMPONENT_END
}