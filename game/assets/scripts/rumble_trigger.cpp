#include "rumble_trigger.hpp"

KK_COMPONENT_IMPL_BEGIN(RumbleTrigger)

KK_COMPONENT_IMPL_END

void RumbleTrigger::OnStart()
{}

void RumbleTrigger::OnUpdate()
{}

void RumbleTrigger::OnTriggerStay(const Collider* i_ownerCollider, const Collider* i_otherCollider)
{
    Inputs::RumbleGamepad(Time::GetDeltaTime());
}

