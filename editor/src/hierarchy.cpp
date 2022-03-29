//
// Created by m.mehalin on 16/03/2022.
//

#include <imgui.h>

#include "game/engine_entity.hpp"
#include "game/entity_manager.hpp"
#include "editor/hierarchy.hpp"

using namespace Editor;


void Hierarchy::OnImGuiRender(Game::EntityManager &entityManager)
{
    ImGui::Begin("Hierarchy");

    if(ImGui::Button("Add entity"))
    {
        std::unique_ptr<Game::EngineEntity> entity = std::make_unique<Game::EngineEntity>();
        entity->GetModel().AddSphereMesh();
        entity->GetModel().AddTextureToMesh("game/assets/gold.jpg", true, 0);
        entityManager.AddEntity(std::move(entity));
    }

    int id = 0;
    auto& entities = entityManager.GetEntities();
    for (const std::unique_ptr<Game::EngineEntity>& entity : entities)
    {
        ImGui::PushID(id);

        std::string label = std::string("Entity_") + std::to_string(id++);
        if (ImGui::Selectable(label.c_str(), label == selectedLabel))
        {
            selected = entity.get();
            selectedLabel = label;
        }

        ImGui::PopID();
    }

    ImGui::End();
}