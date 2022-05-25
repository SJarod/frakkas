#include "game/lowcomponent/collider/sphere_collider.hpp"

#include "collider_creator.hpp"

KK_COMPONENT_IMPL_BEGIN(ColliderCreator)

KK_COMPONENT_IMPL_END

void ColliderCreator::OnStart()
{
    owner.get()->AddComponent<SphereCollider>();
}

void ColliderCreator::OnUpdate()
{}

