//
// Created by flori on 3/23/2022.
//

#include "game/transform.hpp"

#include "game/engine_entity.hpp"

using namespace Game;

Renderer::Model &EngineEntity::GetModel() {
    return model;
}

Game::Transform &EngineEntity::GetTransform() {
    return model.transform;
}

EngineEntity::EngineEntity() {
    transform.parent = &model.transform;
}
