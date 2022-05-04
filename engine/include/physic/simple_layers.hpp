#pragma once

#include <Jolt.h>

#include <Physics/Collision/ObjectLayer.h>
#include <Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Physics/Collision/ContactListener.h>

#include "debug/log.hpp"

JPH_NAMESPACE_BEGIN

    namespace Layers
    {
        static constexpr uint8 NON_MOVING = 0;
        static constexpr uint8 MOVING = 1;
        static constexpr uint8 SENSOR = 2;
        static constexpr uint8 NUM_LAYERS = 3;
    };

    static bool MyObjectCanCollide(ObjectLayer i_object1, ObjectLayer i_object2)
    {
        switch (i_object1)
        {
            case Layers::NON_MOVING: // Collide only with moving
                return i_object2 == Layers::MOVING;
            case Layers::MOVING:  // Collider with everything
            case Layers::SENSOR:
                return true;
            default:
                JPH_ASSERT(false);
                return false;
        }
    }

    namespace BroadPhaseLayers
    {
        static constexpr BroadPhaseLayer NON_MOVING(0);
        static constexpr BroadPhaseLayer MOVING(1);
        static constexpr BroadPhaseLayer SENSOR(2);
        static constexpr uint NUM_LAYERS(2);
    }


    class MyBroadPhaseLayerInterface final : public BroadPhaseLayerInterface
    {
    public:
        MyBroadPhaseLayerInterface()
        {
            objectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
            objectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
            objectToBroadPhase[Layers::SENSOR] = BroadPhaseLayers::SENSOR;
        }

        uint GetNumBroadPhaseLayers() const override
        {
            return BroadPhaseLayers::NUM_LAYERS;
        }

        BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer i_layer) const override
        {
            JPH_ASSERT(i_layer < Layers::NUM_LAYERS);
            return objectToBroadPhase[i_layer];
        }

    private:
        BroadPhaseLayer objectToBroadPhase[Layers::NUM_LAYERS];
    };

    static bool MyBroadPhaseCanCollide(ObjectLayer i_layer1, BroadPhaseLayer i_layer2)
    {
        switch (i_layer1)
        {
            case Layers::NON_MOVING:
                return i_layer2 == BroadPhaseLayers::MOVING;
            case Layers::MOVING:
            case Layers::SENSOR:
                return true;
            default:
                JPH_ASSERT(false);
                return false;
        }
    }

JPH_NAMESPACE_END