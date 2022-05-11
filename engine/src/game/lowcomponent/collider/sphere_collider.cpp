
#include <Jolt.h>

#include <Physics/Collision/Shape/SphereShape.h>
#include <Physics/Body/Body.h>

#include "resources/mesh.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/model.hpp"

#include "game/lowcomponent/collider/sphere_collider.hpp"

KK_COMPONENT_IMPL(SphereCollider)
KK_FIELD_IMPL(SphereCollider, radius, EDataType::FLOAT)

void SphereCollider::UpdateSphereShape()
{
    Vector3 scale = owner.get()->transform.scale.get();
    float fullRadius = radius * Maths::Max(scale.x, scale.y, scale.z);
    if (fullRadius == prevRadius)
        return;

    auto* sphere = new JPH::SphereShape(fullRadius);
    collider->SetShapeInternal(sphere, false);

    prevRadius = fullRadius;
    debugModel.transform.scale = {radius, radius, radius};
}

void SphereCollider::ApplyEditorUpdate(JPH::BodyInterface* i_bodyInterface)
{
    Collider::ApplyEditorUpdate(i_bodyInterface);

    UpdateSphereShape();
}

void SphereCollider::DebugDraw(Renderer::LowLevel::LowRenderer& i_renderer) const
{
    if (!debugModel.mesh)
    {
        debugModel.SetMeshFromFile(Resources::Mesh::sphereColliderMesh);
        debugModel.transform.parent = &owner.get()->transform;
    }

    Resources::Submesh* smesh = debugModel.mesh->submeshes.front().get();

    Matrix4 modelMat = smesh->localTransform *  debugModel.transform.GetModelMatrixUniformScale();

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
