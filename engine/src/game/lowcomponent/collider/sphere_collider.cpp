
#include <Jolt.h>

#include <Physics/Collision/Shape/SphereShape.h>
#include <Physics/Body/Body.h>

#include "resources/mesh.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/model.hpp"

#include "game/lowcomponent/collider/sphere_collider.hpp"

KK_COMPONENT_IMPL(SphereCollider)
KK_FIELD_IMPL(SphereCollider, radius, EDataType::FLOAT, 1)

void SphereCollider::ApplyNewRadius(float i_radius)
{
    prevRadius = i_radius;
    auto* sphere = new JPH::SphereShape(i_radius);
    collider->SetShapeInternal(sphere, false);
    debugModel.transform.scale = {i_radius, i_radius, i_radius};
}

void SphereCollider::ApplyEditorUpdate(JPH::BodyInterface* i_bodyInterface)
{
    Collider::ApplyEditorUpdate(i_bodyInterface);
    if (radius != prevRadius)
        ApplyNewRadius(radius);
}

void SphereCollider::DebugDraw(Renderer::LowLevel::LowRenderer& i_renderer) const
{
    if (!debugModel.mesh)
        debugModel.SetMeshFromFile(Resources::Mesh::sphereColliderMesh);

    Resources::Submesh* smesh = debugModel.mesh->submeshes.front().get();

    Matrix4 modelMat = smesh->localTransform *  debugModel.transform.GetModelMatrix() * owner.get()->transform.GetModelMatrix();

    i_renderer.RenderLines(smesh->gpu.VAO, smesh->vertices.size(), modelMat, 1, {0.5f, 1.f, 0.5f}, false);
}
