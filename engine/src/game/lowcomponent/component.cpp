#include "game/entity.hpp"
#include "game/entity_manager.hpp"
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

Property<Vector3>& Component::Position() const
{
    return owner.get()->transform.position;
}

Property<Vector3>& Component::Scale() const
{
    return owner.get()->transform.scale;
}

Property<Vector3>& Component::Rotation() const
{
    return owner.get()->transform.rotation;
}

EntityContainer& Component::GetEntityContainer() const
{
    return *owner.get()->entityStore;
}



