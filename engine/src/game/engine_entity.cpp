//
// Created by flori on 3/23/2022.
//

#include "../../../physics/include/transform.hpp"

#include "game/engine_entity.hpp"

using namespace Game;

Renderer::Model &EngineEntity::GetModel() {
    return model;
}

Physics::Transform &EngineEntity::GetTransform() {
    return model.transform;
}
