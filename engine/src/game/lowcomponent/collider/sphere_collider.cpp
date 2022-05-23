#include <Jolt/Jolt.h>

#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyInterface.h>

#include "resources/mesh.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/model.hpp"

#include "game/lowcomponent/collider/sphere_collider.hpp"

KK_COMPONENT_IMPL_BEGIN(SphereCollider)
KK_COMPONENT_IMPL_END

void SphereCollider::UpdateSphereShape()
{
    const Vector3& scale = owner.get()->transform.scale;

    float radius = Maths::Max(scale.x, scale.y, scale.z);
    if (radius == prevRadius)
        return;

    auto* sphere = new JPH::SphereShape(radius);
    bodyInterface->SetShape(GetPhysicBodyID(), sphere, false, JPH::EActivation::Activate);

    prevRadius = radius;
}

void SphereCollider::ApplyEntityUpdate()
{
    Collider::ApplyEntityUpdate();

    UpdateSphereShape();
}

void SphereCollider::DebugDraw(LowRenderer& i_renderer, const Game::Transform& i_entityTransform) const
{
    if (!debugModel.mesh)
        debugModel.SetMeshFromFile(Resources::Mesh::sphereColliderMesh);

    Resources::Submesh* smesh = debugModel.mesh->submeshes.front().get();

    Matrix4 modelMat = smesh->localTransform *  i_entityTransform.GetModelMatrix();

    i_renderer.RenderLines(smesh->gpu.VAO, smesh->vertices.size(), modelMat, 1, {0.5f, 1.f, 0.5f}, false);
}

void SphereCollider::OnCollisionEnter(const Collider* i_ownerCollider, const Collider* i_otherCollider)
{
    //Log::Info("Enter Collision !");
}

void SphereCollider::OnCollisionExit(const Collider* i_ownerCollider, const Collider* i_otherCollider)
{
    //Log::Info("Exit collision !");
}

void SphereCollider::OnTriggerEnter(const Collider* i_ownerCollider, const Collider* i_otherCollider)
{
    //Log::Info("Enter Trigger !");
}
