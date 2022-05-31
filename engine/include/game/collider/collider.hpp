#pragma once

#include <Jolt/Jolt.h>

#include <Jolt/Physics/Body/BodyInterface.h>

#include "maths.hpp"

#include "renderer/model.hpp"

#include "physic/layers.hpp"

#include "game/component_generator.hpp"

namespace JPH
{
    class Body;
    class BodyID;
}

namespace Renderer::LowLevel
{
    class LowRenderer;
}

namespace Game
{
    KK_PRIVATE_COMPONENT(Collider)

        Vector3 velocity;
        Vector3 angularVelocity;

        bool trigger = false;
        bool isStatic = true;

        bool constraintPositionX = false;
        bool constraintPositionY = false;
        bool constraintPositionZ = false;

        bool constraintRotationX = false;
        bool constraintRotationY = false;
        bool constraintRotationZ = false;

        void OnEnable() override;
        void OnDisable() override;

        /**
         * @return ID of collider in jolt physic system.
         */
        const JPH::BodyID& GetPhysicBodyID() const;

        /**
         * @brief Apply changes done in the editor update.
         */
        virtual void ApplyEntityUpdate();
        /**
        * @brief Apply changes done in the physic system update.
         * Called only if collider does not have parent and if it s dynamic.
        */
        virtual void ApplyPhysicUpdate();

        /**
         * @brief Send body and interface to the collider for physic system access.
         * @param i_body The collider body.
         * @param i_bodyInterface The collider body interface to change body settings.
         */
        void SetPhysicParameters(JPH::Body* i_body, JPH::BodyInterface* i_bodyInterface);

        /**
         * @brief Draw the collider on the editor scene.
         * @param i_renderer The renderer to render the collider mesh
         * @param i_entityTransform The transform of the owner.
         */
        virtual void
        DebugDraw(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform) const {};

    protected:
        JPH::BodyInterface* bodyInterface = nullptr;
        mutable Renderer::Model debugModel;

        /**
         * Apply physic system transform update to entity's transform (translation and scale)
         */
        void ApplyPhysicTransform() const;
        /**
        * Set component's velocity with physic updated velocity.
        */
        void ApplyPhysicVelocity();
        /**
        * Set component's angular velocity with physic updated angular velocity.
        */
        void ApplyPhysicAngularVelocity();

        /**
         * Apply editor and game transform update to physic body's transform (translation and scale)
         */
        void ApplyEntityTransform();

        /**
        * Set physic body's velocity with this component velocity, maybe updated during game update.
        */
        void ApplyEntityVelocity();
        /**
        * Set physic body's angular velocity with this component angular velocity, maybe updated during game update.
        */
        void ApplyEntityAngularVelocity();
        /**
        * Apply the 'isStatic' boolean to set the physic body as static or not.
        */
        void ApplyStaticState();

        /**
        * Apply the 'trigger' boolean to set the physic body as sensor or not.
        */
        void ApplyTriggerState();

    private:
        JPH::Body* body;

    KK_COMPONENT_END
}
