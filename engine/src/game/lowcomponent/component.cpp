#include "game/lowcomponent/component.hpp"

using namespace Game;

std::vector<ClassMetaData*>& Component::GetRegistry()
{
    static std::vector<ClassMetaData*> componentRegistry;
    return componentRegistry;
}

ClassMetaData* Component::FindMetaData(const std::string& i_compName)
{
    std::vector<ClassMetaData*>& registry = GetRegistry();
    auto it = std::find_if(registry.begin(), registry.end(), [&i_compName](ClassMetaData* md){ return i_compName == md->className;});

    if (it != registry.end())
        return *it;

    return nullptr;
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


