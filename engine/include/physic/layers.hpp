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

    // Check the layer of two objects to tell jolt physics if objects can collide
    bool MyObjectCanCollide(ObjectLayer i_object1, ObjectLayer i_object2);

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
        MyBroadPhaseLayerInterface();

        uint GetNumBroadPhaseLayers() const override;

        BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer i_layer) const override;

    private:
        BroadPhaseLayer objectToBroadPhase[Layers::NUM_LAYERS]{};
    };

    // Check the layer of one object and a broadphase (quad tree) to tell jolt physics if objects can collide
    bool MyBroadPhaseCanCollide(ObjectLayer i_layer1, BroadPhaseLayer i_layer2);

JPH_NAMESPACE_END