
#include <Jolt.h>

#include <Physics/Collision/Shape/BoxShape.h>
#include <Physics/Body/Body.h>

#include "resources/mesh.hpp"
#include "resources/resources_manager.hpp"
#include "renderer/lowlevel/lowrenderer.hpp"

#include "game/lowcomponent/collider/box_collider.hpp"

KK_COMPONENT_IMPL(BoxCollider)

void BoxCollider::UpdateBoxScale()
{
    Vector3 extension = owner.get()->transform.scale;
    if (extension == prevExtension)
        return;

    auto* newBox = new JPH::BoxShape(JPH::Vec3(extension.x, extension.y, extension.z));
    bodyInterface->SetShape(GetPhysicBodyID(), newBox, false, JPH::EActivation::Activate);

    prevExtension = extension;
}

void BoxCollider::ApplyEntityUpdate()
{
    Collider::ApplyEntityUpdate();

    UpdateBoxScale();
}

void BoxCollider::DebugDraw(LowRenderer& i_renderer, const Game::Transform& i_entityTransform) const
{
    if (!debugModel.mesh)
        debugModel.SetMeshFromFile(Resources::Mesh::cubeColliderMesh);

    Resources::Submesh* smesh = debugModel.mesh->submeshes.front().get();

    Matrix4 modelMat = smesh->localTransform *  i_entityTransform.GetModelMatrix();

    i_renderer.RenderLines(smesh->gpu.VAO, smesh->vertices.size(), modelMat, 1, {0.5f, 1.f, 0.5f}, true);
}
