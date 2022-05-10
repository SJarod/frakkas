#include <Tracy.hpp>

#include <Jolt.h>


#include <RegisterTypes.h>
#include <Physics/Collision/Shape/BoxShape.h>
#include <Physics/Collision/Shape/SphereShape.h>
#include <Physics/Body/BodyCreationSettings.h>

#include <thread>

#include "game/time_manager.hpp"
#include "game/lowcomponent/collider/collider.hpp"
#include "debug/log.hpp"

#include "physic/physic_scene.hpp"

using namespace Physic;


std::vector<Game::Collider*> PhysicScene::colliders;

PhysicScene::PhysicScene()
{
    JPH::RegisterTypes();

    tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(16 * 1024 * 1024);
    jobSystem = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, 0);

    contactListener = std::make_unique<JPH::MyContactListener>();

    physicsSystem = std::make_unique<JPH::PhysicsSystem>();
    physicsSystem->Init(physicBodyCount, physicBodyMutexCount, physicMaxBodyPairs, physicMaxContactConstraints,
                        broadPhaseLayerInterface, JPH::MyBroadPhaseCanCollide, JPH::MyObjectCanCollide);

    physicsSystem->SetContactListener(contactListener.get());

    // Call if we added body before
    // physicsSystem->OptimizeBroadPhase();

    bodyInterface = &physicsSystem->GetBodyInterfaceNoLock();

}


void PhysicScene::Update(bool i_gaming) const
{
    ZoneScoped

    float dt = Game::Time::GetDeltaTime();

    for (Game::Collider* collider : colliders)
        collider->ApplyEditorUpdate(bodyInterface);

    if (i_gaming)
    {
        physicsSystem->Update(dt, 1, 1, tempAllocator.get(), jobSystem.get());

        for (Game::Collider* collider : colliders)
            collider->ApplyPhysicUpdate();
    }
}

JPH::Body* PhysicScene::CreateBody(const Vector3& i_scale)
{
    JPH::Vec3 halfExtend = JPH::Vec3(i_scale.x / 2.f,i_scale.y / 2.f,i_scale.z / 2.f);
    return CreateBody(new JPH::BoxShape(halfExtend));
}

JPH::Body* PhysicScene::CreateBody(float i_radius)
{
    return CreateBody(new JPH::SphereShape(i_radius));
}

JPH::Body* PhysicScene::CreateBody(JPH::Shape* io_shape)
{
    // Body settings
    JPH::BodyCreationSettings bodySettings(
        io_shape,
        JPH::Vec3(),
        JPH::Quat::sIdentity(),
        JPH::EMotionType::Dynamic,
        JPH::Layers::MOVING
    );

    JPH::Body* body = bodyInterface->CreateBody(bodySettings);
    bodyInterface->AddBody(body->GetID(), JPH::EActivation::Activate);

    return body;
}

void PhysicScene::RemoveBody(const JPH::BodyID& i_ID)
{
    // Remove body from jolt scene
    bodyInterface->RemoveBody(i_ID);
    // Remove body from frakkas scene
    auto it = std::find_if(colliders.begin(), colliders.end(), [&i_ID](const Game::Collider* collider){ return i_ID == collider->GetPhysicBodyID();});
    if (it != colliders.end())
        colliders.erase(it);
}

void PhysicScene::Clear()
{
    while (!colliders.empty())
        RemoveBody(colliders.front()->GetPhysicBodyID());
}

void PhysicScene::NotifyCollision(ECollisionEvent i_event, const JPH::BodyID& i_body1, const JPH::BodyID& i_body2)
{
    auto findPredicate1 = [&i_body1](const Game::Collider* collider) { return i_body1 == collider->GetPhysicBodyID(); };
    Game::Collider* collider1 = *std::find_if(colliders.begin(), colliders.end(), findPredicate1);

    auto findPredicate2 = [&i_body2](const Game::Collider* collider) { return i_body2 == collider->GetPhysicBodyID(); };
    Game::Collider* collider2 = *std::find_if(colliders.begin(), colliders.end(), findPredicate2);

    collider1->owner.get()->NotifyCollision(i_event, collider1, collider2);
    collider2->owner.get()->NotifyCollision(i_event, collider2, collider1);
}

void PhysicScene::NotifyCollisionExit(const JPH::BodyID& i_body1, const JPH::BodyID& i_body2)
{
    auto findPredicate1 = [&i_body1](const Game::Collider* collider) { return i_body1 == collider->GetPhysicBodyID(); };
    Game::Collider* collider1 = *std::find_if(colliders.begin(), colliders.end(), findPredicate1);

    auto findPredicate2 = [&i_body2](const Game::Collider* collider) { return i_body2 == collider->GetPhysicBodyID(); };
    Game::Collider* collider2 = *std::find_if(colliders.begin(), colliders.end(), findPredicate2);

    // Check if collider is trigger or not
    ECollisionEvent event = collider1->isTrigger || collider2->isTrigger ? ECollisionEvent::TRIGGER_EXIT : ECollisionEvent::COLLISION_EXIT;
    collider1->owner.get()->NotifyCollision(event, collider1, collider2);
    collider2->owner.get()->NotifyCollision(event, collider2, collider1);
}
