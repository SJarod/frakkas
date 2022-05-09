
#include <Jolt.h>

#include <Physics/Collision/Shape/BoxShape.h>
#include <Physics/Body/Body.h>

#include "resources/mesh.hpp"
#include "resources/resources_manager.hpp"
#include "renderer/lowlevel/lowrenderer.hpp"

#include "game/lowcomponent/collider/box_collider.hpp"

KK_COMPONENT_IMPL(BoxCollider)
KK_FIELD_IMPL(BoxCollider, halfExtension, EDataType::FLOAT, 3)

void BoxCollider::UpdateBoxScale()
{
    Vector3 fullExtensionScale = halfExtension * owner.get()->transform.scale.get();
    if (fullExtensionScale == prevExtension)
        return;

    auto* newBox = new JPH::BoxShape(JPH::Vec3(fullExtensionScale.x, fullExtensionScale.y, fullExtensionScale.z));
    collider->SetShapeInternal(newBox, false);

    prevExtension = fullExtensionScale;
    debugModel.transform.scale = halfExtension;
}

void BoxCollider::ApplyEditorUpdate(JPH::BodyInterface* i_bodyInterface)
{
    Collider::ApplyEditorUpdate(i_bodyInterface);

    UpdateBoxScale();
}

void BoxCollider::DebugDraw(LowRenderer& i_renderer) const
{
    if (!debugModel.mesh)
    {
        debugModel.SetMeshFromFile(Resources::Mesh::cubeColliderMesh);
        debugModel.transform.parent = &owner.get()->transform;
    }

    Resources::Submesh* smesh = debugModel.mesh->submeshes.front().get();

    Matrix4 modelMat = smesh->localTransform *  debugModel.transform.GetModelMatrix();

    i_renderer.RenderLines(smesh->gpu.VAO, smesh->vertices.size(), modelMat, 1, {0.5f, 1.f, 0.5f}, true);
}
