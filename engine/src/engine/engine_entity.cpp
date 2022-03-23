//
// Created by flori on 3/23/2022.
//

#include "physics/transform.hpp"

#include "engine/engine_entity.hpp"

using namespace Engine;

const Engine::Model &EngineEntity::GetModel() const {
    return model;
}

Physics::Transform &EngineEntity::GetTransform() {
    return model.transform;
}
