
#include <Jolt.h>

#include <Physics/Body/Body.h>
#include <Physics/Body/BodyInterface.h>

#include "game/transform.hpp"
#include "game/entity.hpp"

#include "game/lowcomponent/collider/collider.hpp"

KK_COMPONENT_IMPL(Collider)
KK_FIELD_IMPL(Collider, isStatic, EDataType::BOOL)
KK_FIELD_IMPL(Collider, trigger, EDataType::BOOL)
KK_FIELD_VIEW_ONLY_IMPL(Collider, velocity, EDataType::FLOAT, 3)
KK_FIELD_VIEW_ONLY_IMPL(Collider, angularVelocity, EDataType::FLOAT, 3)

void Collider::OnEnable()
{

}

void Collider::OnDisable()
{

}

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

void Collider::SetRotation(const Quaternion& i_rot)
{
    collider->SetPositionAndRotationInternal(collider->GetPosition(),{i_rot.x, i_rot.y, i_rot.z, i_rot.w});
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

void Collider::SetTriggerState(bool i_isTrigger, JPH::BodyInterface* i_bodyInterface)
{
    collider->SetIsSensor(i_isTrigger);
}

void Collider::ApplyEditorUpdate(JPH::BodyInterface* i_bodyInterface)
{
    Transform& trs = owner.get()->transform;
    SetPosition(trs.position);
    SetRotation(trs.GetQuaternionRotation());

    SetStaticState(isStatic, i_bodyInterface);

    if (trigger)
        isStatic = true;

    SetTriggerState(trigger, i_bodyInterface);

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
