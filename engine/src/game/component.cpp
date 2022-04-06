#include "game/component.hpp"


using namespace Game;

Component::Component(const std::string &id) : id(id)
{
    enabled.setter = [&](const bool& value)
    {
        if (value && enabled == false)
            OnEnable();
        else if (!value && enabled == true)
            OnDisable();
        enabled.set(value);
    };
    owner.setter = std::bind(&Component::SetOwner, this, std::placeholders::_1);
}

void Component::SetOwner(Entity *entity)
{
    owner.set(entity);
}

