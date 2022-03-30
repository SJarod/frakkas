//
// Created by flori on 3/17/2022.
//

#include "game/component.hpp"

using namespace Game;

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
    this->_owner = owner;
}

bool Component::IsEnabled() const {
    return enabled;
}
