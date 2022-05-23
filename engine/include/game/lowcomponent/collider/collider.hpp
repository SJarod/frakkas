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

        KK_FIELD(Vector3, velocity);
        KK_FIELD(Vector3, angularVelocity);

        KK_FIELD(bool, trigger) = false;
        KK_FIELD(bool, isStatic) = true;

        void OnEnable() override;
        void OnDisable() override;

        /**
         * @return ID of collider in jolt physic system.
         */
        const JPH::BodyID& GetPhysicBodyID() const;

        /**
         * @brief Apply changes done in the editor update (translation, change motion...)
         * @param i_bodyInterface The jolt body interface to change collider layer.
         */
        virtual void ApplyEntityUpdate();
        /**
        * @brief Apply changes done in the physic update.
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
         * @return The position of the collider
         */
        void ApplyPhysicPosition() const;
        /**
         * @return The rotation of the collider
         */
        void ApplyPhysicRotation() const;
        /**
        * @return The linear velocity of the collider.
        */
        void ApplyPhysicVelocity();
        /**
        * @return The angular velocity of the collider.
        */
        void ApplyPhysicAngularVelocity();

        /**
         * @param i_position Apply a position to the collider.
         * In fact, we force the jolt body to be to the input position, and keep the same rotation.
         */
        void ApplyEntityPosition();
        /**
         * @param i_rot Apply a rotation to the collider.
         * In fact, we force the jolt body to be to the input rotation, and keep the same position.
         */
        void ApplyEntityRotation();

        /**
        * @param i_velocity Apply this new input velocity to the collider.
        */
        void ApplyEntityVelocity();
        /**
        * @param i_angVelocity Apply this new input velocity to the collider.
        */
        void ApplyEntityAngularVelocity();
        /**
        * @brief Update the motion type and the layer if static bool is changed.
        * @param i_isStatic The static state of the body.
        * @param i_bodyInterface The Jolt body interface to change body object layer.
        */
        void ApplyStaticState();

        /**
        * @brief Update the collision type and the layer if static bool is changed.
        * @param i_isSensor The sensor state of the body.
        */
        void ApplyTriggerState();

    private:
        JPH::Body* body;

    KK_COMPONENT_END
}
