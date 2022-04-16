#include <imgui.h>
#include <string>

#include "game/entity.hpp"
#include "game/drawable.hpp"
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

    int id = 0;
    auto& entities = i_entityManager.GetEntities();
    for (const std::unique_ptr<Game::Entity>& entity : entities)
    {
        ImGui::PushID(id);

        std::string_view label = entity->name;
        if (ImGui::Selectable(label.data(), label == selectedLabel))
        {
            selected = entity.get();
            selectedLabel = label;
        }

        ImGui::PopID();
    }

    ImGui::End();
}
