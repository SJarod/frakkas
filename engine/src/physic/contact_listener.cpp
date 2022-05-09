#include <Jolt.h>

#include "debug/log.hpp"

#include "physic/physic_scene.hpp"
#include "physic/contact_listener.hpp"

JPH::ValidateResult	JPH::MyContactListener::OnContactValidate(const Body& i_body1, const Body& i_body2, const CollideShapeResult& i_collisionResult)
{



	return ValidateResult::AcceptAllContactsForThisBodyPair;
}

// OnCollisionEnter
void JPH::MyContactListener::OnContactAdded(const Body& i_body1, const Body& i_body2, const ContactManifold& i_manifold, ContactSettings& io_settings)
{
	if (i_body1.IsSensor() || i_body2.IsSensor())
		Physic::PhysicScene::NotifyCollision(Physic::ECollisionEvent::TRIGGER_ENTER, i_body1.GetID(),i_body2.GetID());
	else
		Physic::PhysicScene::NotifyCollision(Physic::ECollisionEvent::COLLISION_ENTER, i_body1.GetID(),i_body2.GetID());
}

// OnCollisionStay
void JPH::MyContactListener::OnContactPersisted(const Body& i_body1, const Body& i_body2, const ContactManifold& i_manifold, ContactSettings& io_settings)
{
	if (i_body1.IsSensor() || i_body2.IsSensor())
		Physic::PhysicScene::NotifyCollision(Physic::ECollisionEvent::TRIGGER_STAY, i_body1.GetID(),i_body2.GetID());
	else
		Physic::PhysicScene::NotifyCollision(Physic::ECollisionEvent::COLLISION_STAY, i_body1.GetID(),i_body2.GetID());
}

// OnColllisionExit
void JPH::MyContactListener::OnContactRemoved(const SubShapeIDPair& i_subShapePair)
{
	Physic::PhysicScene::NotifyCollisionExit(i_subShapePair.GetBody1ID(), i_subShapePair.GetBody2ID());
}
