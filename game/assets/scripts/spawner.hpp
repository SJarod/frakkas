#pragma once

#include "game/lowcomponent/collider/sphere_collider.hpp"

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(Spawner);

public:
        SphereCollider* rb = nullptr;

    private:
        bool canSpawn = true;
        float curSpawnTime = 0.f;
        float spawnTime = 5.f;

        void OnStart() override;

        void OnUpdate() override;

        void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        void OnTriggerExit(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        void Spawn();

    KK_COMPONENT_END
}