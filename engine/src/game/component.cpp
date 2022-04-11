#include "game/component.hpp"


using namespace Game;



Component::Component()
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
    enabled = true;
}

void Component::SetOwner(Entity* entity) 
{
    owner.set(entity);
}
