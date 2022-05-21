
#include <Jolt.h>

#include <Physics/Body/Body.h>

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
    bodyInterface->ActivateBody(GetPhysicBodyID());
}

void Collider::OnDisable()
{
    bodyInterface->DeactivateBody(GetPhysicBodyID());
}

void Collider::ApplyPhysicPosition() const
{
    JPH::Vec3 pos = bodyInterface->GetPosition(GetPhysicBodyID());
    Position() = {pos.GetX(), pos.GetY(), pos.GetZ()};
}

void Collider::ApplyPhysicRotation() const
{
    JPH::Quat rot = bodyInterface->GetRotation(GetPhysicBodyID());
    Rotation() = -Quaternion(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW()).QuatToEuler();
}

void Collider::ApplyPhysicVelocity()
{
    JPH::Vec3 vel = bodyInterface->GetLinearVelocity(GetPhysicBodyID());
    velocity = Vector3(vel.GetX(), vel.GetY(), vel.GetZ());
}

void Collider::ApplyPhysicAngularVelocity()
{
    JPH::Vec3 vel = bodyInterface->GetAngularVelocity(GetPhysicBodyID());
    angularVelocity = {vel.GetX(), vel.GetY(), vel.GetZ()};
}

void Collider::ApplyEntityPosition()
{
    JPH::Vec3 pos = { Position().get().x, Position().get().y, Position().get().z};
    bodyInterface->SetPosition(GetPhysicBodyID(), pos, JPH::EActivation::Activate);
}

void Collider::ApplyEntityRotation()
{
    Quaternion r = owner.get()->transform.GetQuaternionRotation();
    JPH::Quat rot = {r.x, r.y ,r.z, r.w};
    bodyInterface->SetRotation(GetPhysicBodyID(), rot, JPH::EActivation::Activate);
}

void Collider::SetPhysicParameters(JPH::Body* i_body, JPH::BodyInterface* i_bodyInterface)
{
    body = i_body;
    bodyInterface =  i_bodyInterface;
}

void Collider::ApplyEntityVelocity()
{
    JPH::Vec3 vel = { velocity.x,velocity.y, velocity.z };
    bodyInterface->SetLinearVelocity(GetPhysicBodyID(), vel);
}

void Collider::ApplyEntityAngularVelocity()
{
    JPH::Vec3 vel = { angularVelocity.x,angularVelocity.y, angularVelocity.z };
    bodyInterface->SetAngularVelocity(GetPhysicBodyID(), vel);
}

void Collider::ApplyStaticState()
{
    if (isStatic)
    {
        bodyInterface->SetMotionType(GetPhysicBodyID(), JPH::EMotionType::Static, JPH::EActivation::DontActivate);
        bodyInterface->SetObjectLayer(GetPhysicBodyID(), JPH::Layers::NON_MOVING);
    }
    else
    {
        bodyInterface->SetMotionType(GetPhysicBodyID(), JPH::EMotionType::Dynamic, JPH::EActivation::Activate);
        bodyInterface->SetObjectLayer(GetPhysicBodyID(), JPH::Layers::MOVING);
    }
}

void Collider::ApplyTriggerState()
{
    body->SetIsSensor(trigger);
}

void Collider::ApplyEntityUpdate()
{
    ApplyEntityPosition();
    ApplyEntityRotation();
    ApplyEntityVelocity();
    ApplyEntityAngularVelocity();

    ApplyStaticState();
    if (trigger)
        isStatic = true;

    ApplyTriggerState();
}

void Collider::ApplyPhysicUpdate()
{
    ApplyPhysicPosition();
    ApplyPhysicRotation();
    ApplyPhysicVelocity();
    ApplyPhysicAngularVelocity();
}

const JPH::BodyID& Collider::GetPhysicBodyID() const
{
    return body->GetID();
}
