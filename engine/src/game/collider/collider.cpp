#include <Jolt/Jolt.h>

#include <Jolt/Physics/Body/Body.h>

#include "game/transform.hpp"
#include "game/entity.hpp"

#include "game/collider/collider.hpp"

KK_COMPONENT_IMPL_BEGIN(Collider)

    KK_FIELD_PUSH(Collider, isStatic, EDataType::BOOL)
    KK_FIELD_PUSH(Collider, trigger, EDataType::BOOL)

    KK_FIELD_PUSH_TEXT("Constraints:")
    KK_FIELD_TOOLTIP("Prevent update for selected coordinates.")

    KK_FIELD_PUSH(Collider, constraintPositionX, EDataType::BOOL) KK_FIELD_RENAME("posX")
    KK_FIELD_PUSH(Collider, constraintPositionY, EDataType::BOOL) KK_FIELD_RENAME("posY")
    KK_FIELD_SAMELINE
    KK_FIELD_PUSH(Collider, constraintPositionZ, EDataType::BOOL) KK_FIELD_RENAME("posZ")
    KK_FIELD_SAMELINE

    KK_FIELD_PUSH(Collider, constraintRotationX, EDataType::BOOL) KK_FIELD_RENAME("rotX")
    KK_FIELD_PUSH(Collider, constraintRotationY, EDataType::BOOL) KK_FIELD_RENAME("rotY")
    KK_FIELD_SAMELINE
    KK_FIELD_PUSH(Collider, constraintRotationZ, EDataType::BOOL) KK_FIELD_RENAME("rotZ")
    KK_FIELD_SAMELINE

    KK_FIELD_PUSH(Collider, velocity, EDataType::FLOAT)
    KK_FIELD_COUNT(3)
    KK_FIELD_VIEWONLY

    KK_FIELD_PUSH(Collider, angularVelocity, EDataType::FLOAT)
    KK_FIELD_COUNT(3)
    KK_FIELD_VIEWONLY

KK_COMPONENT_IMPL_END

void Collider::OnEnable()
{
    // Not working
    //bodyInterface->ActivateBody(GetPhysicBodyID());
}

void Collider::OnDisable()
{
    // Not working
    //bodyInterface->DeactivateBody(GetPhysicBodyID());

    bodyInterface->SetObjectLayer(GetPhysicBodyID(), JPH::Layers::DISABLE);
}

void Collider::ApplyPhysicTransform() const
{
    JPH::Vec3 pos = bodyInterface->GetPosition(GetPhysicBodyID());
    Position() = {
            constraintPositionX ? Position().get().x : pos.GetX(),
            constraintPositionY ? Position().get().y : pos.GetY(),
            constraintPositionZ ? Position().get().z : pos.GetZ()
    };

    JPH::Quat rot = bodyInterface->GetRotation(GetPhysicBodyID());
    Vector3 eulerRotation = Quaternion(-rot.GetX(), -rot.GetY(), -rot.GetZ(), rot.GetW()).QuatToEuler();
    Rotation() = {
            constraintRotationX ? Rotation().get().x : eulerRotation.x,
            constraintRotationY ? Rotation().get().y : eulerRotation.y,
            constraintRotationZ ? Rotation().get().z : eulerRotation.z
    };
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

void Collider::ApplyEntityTransform()
{
    Matrix4 world = GetTransform().GetWorldMatrix();
    world = world.OrthoNormalize();

    /// Global Position
    Vector3 p = world.DecomposeTranslation();
    bodyInterface->SetPosition(GetPhysicBodyID(), { p.x, p.y, p.z}, JPH::EActivation::Activate);

    /// Rotation
    Quaternion q = Quaternion::QuatFromMatrix(world).Normalize();
    bodyInterface->SetRotation(GetPhysicBodyID(), {-q.x, -q.y , -q.z, q.w}, JPH::EActivation::Activate);
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
    if (trigger)
        bodyInterface->SetObjectLayer(GetPhysicBodyID(), JPH::Layers::SENSOR);


    body->SetIsSensor(trigger);
}

void Collider::ApplyEntityUpdate()
{
    ApplyEntityTransform();
    ApplyEntityVelocity();
    ApplyEntityAngularVelocity();

    if (trigger)
        isStatic = true;

    if (enabled)
    {
        ApplyStaticState();
        ApplyTriggerState();
    }
}

void Collider::ApplyPhysicUpdate()
{
    ApplyPhysicTransform();
    ApplyPhysicVelocity();
    ApplyPhysicAngularVelocity();
}

const JPH::BodyID& Collider::GetPhysicBodyID() const
{
    return body->GetID();
}
