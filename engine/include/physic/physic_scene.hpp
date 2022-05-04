#pragma once

#include <memory>

#include <Jolt.h>

#include <Core/TempAllocator.h>
#include <Core/JobSystemThreadPool.h>
#include <Physics/PhysicsSettings.h>
#include <Physics/Body/BodyManager.h>
#include <Physics/PhysicsSystem.h>

#include "maths.hpp"
#include "simple_layers.hpp"

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

    private:
        std::unique_ptr<JPH::TempAllocator> tempAllocator = nullptr;
        std::unique_ptr<JPH::JobSystem> jobSystem = nullptr;
        std::unique_ptr<JPH::PhysicsSystem> physicsSystem = nullptr;
        JPH::BodyInterface* bodyInterface = nullptr;

        JPH::MyBroadPhaseLayerInterface broadPhaseLayerInterface;

    };
}