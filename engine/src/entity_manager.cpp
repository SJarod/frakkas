//
// Created by flori on 3/17/2022.
//

#include "entity.hpp"

#include "engine/entity_manager.hpp"

using namespace Engine;

std::shared_ptr<EntityManager> EntityManager::instance = nullptr;

EntityManager &EntityManager::GetInstance() {
    if (!instance)
    {
        struct MakeSharedEnabler : public EntityManager {};
        instance = std::make_shared<MakeSharedEnabler>();
    }

    return *instance;
}

void EntityManager::UpdateAndRender() {

    EntityManager& self = GetInstance();

    for(auto & entity : self.entities)
        entity->Update();
}

void EntityManager::AddEntity(std::unique_ptr<Entity> i_entity) {

    EntityManager& self = GetInstance();

    self.entities.push_back(std::move(i_entity));
    self.entities.back()->Start();
}

void EntityManager::Init() {
    std::unique_ptr<Entity> entity = std::make_unique<Entity>();
    GetInstance().AddEntity(std::move(entity));
}
