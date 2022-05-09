// REPLACE '$name' BY COMPONENT NAME
#pragma once

#include "game/component_generator.hpp"

///// Component class already have useful methods to override :
/// Start(), Update(), OnDestroy(), OnEnable(), OnDisable()
/// OnCollisionEnter(Collider* owner, Collider* other), OnCollisionStay(Collider* owner, Collider* other), OnCollisionExit(Collider* owner, Collider* other)
/// OnTriggerEnter(Collider* owner, Collider* other), OnTriggerStay(Collider* owner, Collider* other), OnTriggerExit(Collider* owner, Collider* other)

namespace Game
{
    KK_COMPONENT($name);
        
        void Start() override;

        void Update() override;

    KK_COMPONENT_END
}
