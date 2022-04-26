#include "game/entity.hpp"

#include "game/lowcomponent/drawable.hpp"

KK_COMPONENT_IMPL(Drawable)

void Drawable::SetOwner(Entity* owner)
{
    model.transform.parent = &owner->transform;
    Component::SetOwner(owner);
}
