#include "game/component.hpp"


using namespace Game;

void Component::SetOwner(Entity *entity) {
    owner.set(entity);
}

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

    owner.setter = [&](Entity* value)
    {
        SetOwner(value);
    };
}
