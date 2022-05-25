#include "rumble_trigger.hpp"

KK_COMPONENT_IMPL_BEGIN(RumbleTrigger)
    KK_FIELD_PUSH(RumbleTrigger, rumblePower, EDataType::FLOAT)
    KK_FIELD_RANGE(0.f, 1.f)
KK_COMPONENT_IMPL_END

void RumbleTrigger::OnStart()
{
    startPosition = Position();
}

void RumbleTrigger::OnUpdate()
{
    if (rumbling)
    {
        Position() = startPosition + Vector3::right * Maths::Lerp(0.f, 2.f, Maths::Sin(Time::GetTime()));
    }
}

void RumbleTrigger::OnTriggerStay(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    Inputs::RumbleGamepad(Time::GetDeltaTime(), rumblePower);
    Log::Info("Stay");
}

void RumbleTrigger::OnTriggerExit(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    rumbling = false;
    Log::Info("Exit");
}

void RumbleTrigger::OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    rumbling = true;
    Log::Info("Enter");
}

void RumbleTrigger::OnCollisionStay(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    //Inputs::RumbleGamepad(Time::GetDeltaTime(), rumblePower);
}

void RumbleTrigger::OnCollisionEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    Inputs::RumbleGamepadPro(0.5f, rumblePower);
    Log::Info("Ouch !");
}

void RumbleTrigger::OnCollisionExit(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    Inputs::RumbleGamepadPro(0.5f, 0.f, rumblePower);
    Log::Info("Pwouah !");
}


