#include "game/lowcomponent/component.hpp"

using namespace Game;

std::vector<ClassMetaData*>& Component::GetRegistry()
{
    static std::vector<ClassMetaData*> componentRegistry;
    return componentRegistry;
}

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


