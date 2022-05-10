#pragma once

#include <memory>

#include <Jolt.h>

#include <Core/TempAllocator.h>
#include <Core/JobSystemThreadPool.h>
#include <Physics/PhysicsSettings.h>
#include <Physics/Body/BodyManager.h>
#include <Physics/PhysicsSystem.h>

#include "maths.hpp"
#include "layers.hpp"
#include "contact_listener.hpp"

namespace JPH
{
    class BodyInterface;
    class Body;
}

using uint = unsigned int;

namespace Game
{
    class Collider;
}

namespace Physic
{
    enum class EMotionType;
    class BoxCollider;
    class SphereCollider;

    static constexpr uint physicBodyCount = 10240;
    static constexpr uint physicBodyMutexCount = 0; // Autodetect
    static constexpr uint physicMaxBodyPairs = 65536;
    static constexpr uint physicMaxContactConstraints = 10240;

    enum class ECollisionEvent
    {
        COLLISION_ENTER,
        COLLISION_STAY,
        COLLISION_EXIT,
        TRIGGER_ENTER,
        TRIGGER_STAY,
        TRIGGER_EXIT
    };

    class PhysicScene
    {
    public:
        PhysicScene();
        ~PhysicScene() = default;

        static std::vector<Game::Collider*> colliders;

        /**
         * @brief Update physic scene (apply velocity and process collision), and Update colliders transform.
         * @param i_gaming The current gaming state of the engine.
         */
        void Update(bool i_gaming = true) const;

        /**
         * @brief Generate a jolt physic body with box shape. Added to jolt's BodyInterface
         * @param i_scale The scale of the box.
         * @return The generated body.
         */
        JPH::Body* CreateBody(const Vector3& i_scale = Vector3(1.f, 1.f, 1.f));
        /**
         * @brief Generate a jolt physic body with sphere shape. Added to jolt's BodyInterface.
         * @param i_radius The radius of the sphere.
         * @return The generated body.
         */
        JPH::Body* CreateBody(float i_radius = 1.f);
        /**
        * @brief Generate a jolt physic body from a generated shape.
        * @param i_shape The generated inptu shape.
        * @return The generated body.
        */
        JPH::Body* CreateBody(JPH::Shape* i_shape);

        /**
         * @brief Remove a physic body from physic scene and jolt body interface.
         * @param i_ID The ID of the physic body.
         */
        void RemoveBody(const JPH::BodyID& i_ID);

        /**
         * @brief Clear the colliders list, and remove all physic body from physic scene.
         */
        void Clear();

        /**
         * @brief Notify the physic scene that a collision happened, then notify the concerned entities.
         * @param i_event The collision event type.
         * @param i_body1 The physic system ID of the first entity which collides.
         * @param i_body1 The physic system ID of the second entity which collides.
         */
        static void NotifyCollision(ECollisionEvent i_event, const JPH::BodyID& i_body1, const JPH::BodyID& i_body2);

        /**
         * @brief Check and notify the physic scene that a collision finished, then notify the concerned entities.
         * @param i_body1 The physic system ID of the first entity which collides.
         * @param i_body1 The physic system ID of the second entity which collides.
         */
        static void NotifyCollisionExit(const JPH::BodyID& i_body1, const JPH::BodyID& i_body2);


    private:
        std::unique_ptr<JPH::TempAllocator> tempAllocator = nullptr;
        std::unique_ptr<JPH::JobSystem> jobSystem = nullptr;
        std::unique_ptr<JPH::PhysicsSystem> physicsSystem = nullptr;
        std::unique_ptr<JPH::MyContactListener> contactListener = nullptr;
        JPH::BodyInterface* bodyInterface = nullptr;

        JPH::MyBroadPhaseLayerInterface broadPhaseLayerInterface;

    };
}