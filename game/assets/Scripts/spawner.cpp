#include <random>

#include "golem.hpp"
#include "skeleton.hpp"

#include "spawner.hpp"

KK_COMPONENT_IMPL_BEGIN(Spawner)

    KK_FIELD_PUSH(Spawner, spawnTime, EDataType::FLOAT)

KK_COMPONENT_IMPL_END

void Spawner::OnStart()
{
    owner.get()->name = "Spawner_" + std::to_string(owner.get()->GetID());

    rb = owner.get()->AddComponent<SphereCollider>();
    rb->trigger = true;
}

void Spawner::OnUpdate()
{
    curSpawnTime += Time::GetDeltaTime();
    if (curSpawnTime >= spawnTime && canSpawn)
    {
        Spawn();
        curSpawnTime = 0.f;
    }
}

void Spawner::OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (i_otherCollider.GetID() == "player")
        canSpawn = false;
}

void Spawner::OnTriggerExit(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (i_otherCollider.GetID() == "player")
        canSpawn = true;
}

void Spawner::Spawn()
{
    std::random_device randDevice;
    std::mt19937_64 gen(randDevice());
    std::discrete_distribution<> discreteDistri({75, 15});

    Entity* en = GetEntityContainer().CreateEntity();

    if (discreteDistri(gen) == 0)
    {
        en->AddComponent<Skeleton>();
        en->transform.position = Position().get();
    }
    else
    {
        en->AddComponent<Golem>();
        en->transform.position = Position().get();
    }
}