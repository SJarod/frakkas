//
// Created by flori on 3/23/2022.
//

#include <imgui.h>

#include "game/engine_entity.hpp"

using namespace Game;

Renderer::Model &EngineEntity::GetModel() {
    return model;
}

Game::Transform &EngineEntity::GetModelTransform() {
    return model.transform;
}

EngineEntity::EngineEntity() {
    model.transform.parent = &transform;
}
    
void EngineEntity::Edit() {
    Entity::Edit();

    model.Edit();
}
