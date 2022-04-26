#include <imgui.h>
#include <string>

#include "game/entity.hpp"
#include "game/lowcomponent/drawable.hpp"
#include "game/entity_manager.hpp"

#include "editor/hierarchy.hpp"

using namespace Editor;

void Hierarchy::OnImGuiRender(Game::EntityManager& i_entityManager)
{
    ImGui::Begin("Hierarchy");

    if(ImGui::Button("Add entity"))
    {
        static int crID = 0;
        Game::Entity* entity = i_entityManager.CreateEntity(std::string("new_"+std::to_string(crID++)));
        auto drawable = entity->AddComponent<Game::Drawable>();
        drawable->model.AddSphereMesh();
        drawable->model.AddTextureToMesh("game/assets/gold.jpg", true, 0);
    }

    int id = 0, removeID = -1;
    auto& entities = i_entityManager.GetEntities();
    if (ImGui::BeginTable("Entities list", 2))
    {
        ImGui::TableSetupColumn("one", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("two", ImGuiTableColumnFlags_WidthFixed, 20.0f);

        for (const std::unique_ptr<Game::Entity>& entity : entities)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::PushID(id++);

            std::string_view label = entity->name;
            if (ImGui::Selectable(label.data(), label == selectedLabel))
            {
                selected = entity.get();
                selectedLabel = label;
            }
            ImGui::TableSetColumnIndex(1);

            if (ImGui::Button("X"))
            {
                if (selected == entity.get()) selected = nullptr;
                removeID = id-1;
            }

            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    if (removeID != -1)
        i_entityManager.RemoveEntityAt(removeID);

    ImGui::End();
}
