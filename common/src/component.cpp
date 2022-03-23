//
// Created by flori on 3/17/2022.
//

#include "component.hpp"

void Component::Enable() {
    if (!enabled)
    {
        enabled = true;
        OnEnable();
    }
}

void Component::Disable() {
    if (enabled)
    {
        enabled = false;
        OnDisable();
    }
}

void Component::SetOwner(Entity *owner) {
    this->owner = owner;
}

bool Component::IsEnabled() const {
    return enabled;
}
