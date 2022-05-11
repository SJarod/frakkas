#pragma once

#include "game/component_generator.hpp"
#include "collider.hpp"
#include "renderer/lowlevel/lowrenderer.hpp"

namespace Game
{
    KK_COMPONENT_FROM(SphereCollider, Collider);

        void ApplyEditorUpdate(JPH::BodyInterface* i_bodyInterface) override;

        void DebugDraw(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform) const override;

        void OnCollisionEnter(const Collider* i_ownerCollider, const Collider* i_otherCollider) override;

        void OnCollisionExit(const Collider* i_ownerCollider, const Collider* i_otherCollider) override;

        void OnTriggerEnter(const Collider* i_ownerCollider, const Collider* i_otherCollider) override;

    private:
        float prevRadius = 1.f;
        /**
         * Check collider scale, and create new scaled sphere if needed.
         */
        void UpdateSphereShape();

    KK_COMPONENT_END
}
