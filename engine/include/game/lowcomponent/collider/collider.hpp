#pragma once

#include <Jolt.h>

#include "maths.hpp"

#include "renderer/model.hpp"

#include "physic/layers.hpp"

#include "game/component_generator.hpp"


namespace JPH
{
    class Body;
    class BodyID;
    class BodyInterface;
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

        KK_FIELD(bool, isTrigger) = false;
        KK_FIELD(bool, isStatic) = true;

        /**
         * @return ID of collider in jolt physic system.
         */
        const JPH::BodyID& GetPhysicBodyID() const;

        /**
         * @brief Apply changes done in the editor update (translation, change motion...)
         * @param i_bodyInterface The jolt body interface to change collider layer.
         */
        virtual void ApplyEditorUpdate(JPH::BodyInterface* i_bodyInterface);
        /**
        * @brief Apply changes done in the physic update.
        */
        virtual void ApplyPhysicUpdate();

        /**
         * Give a constructed body to this collider.
         * @param i_collider The constructed body.
         */
        void SetCollider(JPH::Body* i_collider);

        /**
         * Draw the collider on the scene.
         */
        virtual void DebugDraw(Renderer::LowLevel::LowRenderer& i_renderer) const {};

    protected:
        JPH::Body* collider = nullptr;
        mutable Renderer::Model debugModel;

        /**
         * @return The position of the collider
         */
        Vector3 GetPosition() const;
        /**
         * @return The rotation of the collider
         */
        Quaternion GetRotation() const;
        /**
        * @return The linear velocity of the collider.
        */
        Vector3 GetVelocity() const;
        /**
        * @return The angular velocity of the collider.
        */
        Vector3 GetAngularVelocity() const;

        /**
         * @param i_position Apply a position to the collider.
         * In fact, we force the jolt body to be to the input position, and keep the same rotation.
         */
        void SetPosition(const Vector3& i_position);
        /**
         * @param i_rotation Apply a rotation to the collider.
         * In fact, we force the jolt body to be to the input rotation, and keep the same position.
         */
        void SetRotation(const Quaternion& i_rotation);

        /**
        * @param i_velocity Apply this new input velocity to the collider.
        */
        void SetVelocity(const Vector3& i_velocity);
        /**
        * @param i_angVelocity Apply this new input velocity to the collider.
        */
        void SetAngularVelocity(const Vector3& i_angVelocity);
        /**
        * @brief Update the motion type and the layer if static bool is changed.
        * @param i_isStatic The static state of the body.
        * @param i_bodyInterface The Jolt body interface to change body object layer.
        */
        void SetStaticState(bool i_isStatic, JPH::BodyInterface* i_bodyInterface);

        /**
        * @brief Update the collision type and the layer if static bool is changed.
        * @param i_isSensor The sensor state of the body.
        * @param i_bodyInterface The Jolt body interface to change body object layer.
        */
        void SetTriggerState(bool i_isTrigger, JPH::BodyInterface* i_bodyInterface);

    KK_COMPONENT_END
}
