
#include <Jolt.h>

#include <Physics/Body/Body.h>
#include <Physics/Body/BodyInterface.h>

#include "game/time_manager.hpp"
#include "game/transform.hpp"
#include "game/entity.hpp"

#include "game/lowcomponent/collider/collider.hpp"

using namespace Game;

Vector3 Collider::GetPosition() const
{
    JPH::Vec3 pos = collider->GetPosition();
    return Vector3(pos.GetX(), pos.GetY(), pos.GetZ());
}

Quaternion Collider::GetRotation() const
{
    JPH::Quat rot = collider->GetRotation();
    return Quaternion(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW());
}

Vector3 Collider::GetVelocity() const
{
    JPH::Vec3 vel = collider->GetLinearVelocity();
    return Vector3(vel.GetX(), vel.GetY(), vel.GetZ());
}

Vector3 Collider::GetAngularVelocity() const
{
    JPH::Vec3 vel = collider->GetAngularVelocity();
    return Vector3(vel.GetX(), vel.GetY(), vel.GetZ());
}

void Collider::SetPosition(const Vector3& i_position)
{
    JPH::Vec3 pos = { i_position.x, i_position.y, i_position.z};
    collider->SetPositionAndRotationInternal(pos, collider->GetRotation());
}

void Collider::SetRotation(const Quaternion& i_rotation)
{
    JPH::Quat rot = { i_rotation.x, i_rotation.y, i_rotation.z, i_rotation.w};
    collider->SetPositionAndRotationInternal(collider->GetPosition(), rot);
}

void Collider::SetCollider(JPH::Body* i_collider)
{
    collider = i_collider;
}

void Collider::SetVelocity(const Vector3& i_velocity)
{
    JPH::Vec3 vel = { i_velocity.x,i_velocity.y, i_velocity.z };
    collider->SetLinearVelocity(vel);
}

void Collider::SetAngularVelocity(const Vector3& i_angVelocity)
{
    JPH::Vec3 vel = { i_angVelocity.x,i_angVelocity.y, i_angVelocity.z };
    collider->SetAngularVelocity(vel);
}

void Collider::SetStaticState(bool i_isStatic, JPH::BodyInterface* i_bodyInterface)
{
    if (i_isStatic)
    {
        collider->SetMotionType(JPH::EMotionType::Static);
        i_bodyInterface->SetObjectLayer(GetPhysicBodyID(), JPH::Layers::NON_MOVING);
    }
    else
    {
        collider->SetMotionType(JPH::EMotionType::Dynamic);
        i_bodyInterface->SetObjectLayer(GetPhysicBodyID(), JPH::Layers::MOVING);
    }
}


void Collider::Update()
{
    Transform& trs = owner.get()->transform;
    trs.position = trs.position.get() + velocity * Time::GetFixedDeltaTime();
    trs.rotation = trs.rotation.get() + angularVelocity * Time::GetFixedDeltaTime();
}

void Collider::ApplyEditorUpdate(JPH::BodyInterface* i_bodyInterface)
{
    Transform& trs = owner.get()->transform;
    SetPosition(trs.position);
    SetRotation(Quaternion::QuatFromEuler(trs.rotation.get().x, trs.rotation.get().y, trs.rotation.get().z));

    SetStaticState(isStatic, i_bodyInterface);

    if (!isStatic)
    {
        SetVelocity(velocity);
        SetAngularVelocity(angularVelocity);
    }
}

void Collider::ApplyPhysicUpdate()
{
    Transform& trs = owner.get()->transform;
    trs.position = GetPosition();
    velocity = GetVelocity();
    //TODO Quat to Euler angles
    // trs.rotation = GetRotation();

}

const JPH::BodyID& Collider::GetPhysicBodyID() const
{
    return collider->GetID();
}
