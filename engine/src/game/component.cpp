#include "game/component.hpp"


using namespace Game;

void Component::Enable()
{
    if (!enabled)
    {
        enabled = true;
        OnEnable();
    }
}

void Component::Disable()
{
    if (enabled)
    {
        enabled = false;
        OnDisable();
    }
}

void Component::SetOwner(Entity *owner) {
    this->owner.set(owner);
}

bool Component::IsEnabled() const
{
    return enabled;
}

Component::Component(const std::string &id)  : id(id)
{
    owner.setter = std::bind(&Component::SetOwner, this, std::placeholders::_1);
}
