#include <imgui.h>
#include <string>

#include "game/lowcomponent/drawable.hpp"
#include "game/entity_manager.hpp"

#include "helpers/game_edit.hpp"

#include "editor/hierarchy.hpp"

using namespace Editor;

inline std::string BuildEntityLabel(const std::string& name, const Game::EntityIdentifier& id)
{
    return name + "###" + std::to_string(id);
}

void Hierarchy::OnImGuiRender(Game::EntityManager& io_entityManager)
{
    ImGui::Begin("Hierarchy");

    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && selected) // Will edit name if double clicked
        editSelectedName = true;

    if(ImGui::Button("Add entity"))
    {
        Game::Entity* entity = io_entityManager.CreateEntity();
        auto drawable = entity->AddComponent<Game::Drawable>();
        drawable->model.AddSphereMesh();
        drawable->model.AddTextureToMesh("game/assets/gold.jpg", true, 0);
    }

    // reset remove id
    removeID = -1;

    // TODO : Find a better place to unset parent
    if (selected && selected->unsettingParent)
    {
        selected->unsettingParent = false;
        io_entityManager.UnsetEntityParent(*selected);
    }

    if (ImGui::BeginTable("Entities list", 2))
    {
        ImGui::TableSetupColumn("one", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("two", ImGuiTableColumnFlags_WidthFixed, 20.0f);

        RenderEntitiesHierarchy(io_entityManager, io_entityManager.GetRootEntities());

        ImGui::EndTable();
    }
     
    // Remove an entity if remove ID had been modify
    if (removeID != -1)
        io_entityManager.RemoveEntityAt(removeID);


    ImGui::End();
}

void Hierarchy::RenderEntitiesHierarchy(Game::EntityManager& io_entityManager, const std::unordered_map<Game::EntityIdentifier, Game::Entity*>& io_entities)
{
    for (auto& pair : io_entities)
    {
        RenderEntity(io_entityManager, *pair.second);
    }
}

void Editor::Hierarchy::RenderEntitiesHierarchy(Game::EntityManager& io_entityManager, const std::list<Game::Entity*>& io_entities)
{
    for (Game::Entity* entity : io_entities)
    {
        RenderEntity(io_entityManager, *entity);
    }
}

void Hierarchy::RenderEntity(Game::EntityManager& io_entityManager, Game::Entity& io_entity)
{
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::PushID(io_entity.GetID());

    bool treeOpen = false;

    std::string label = BuildEntityLabel(io_entity.name, io_entity.GetID()); // '###' avoid entity with the same name to be multi-selected

    bool isSelectedEntity = label == selectedLabel;
    if (editSelectedName && isSelectedEntity)
    {
        bool pressEnter = Helpers::Edit(selected->name, "Rename"); // Rename

        label = selectedLabel = BuildEntityLabel(selected->name, io_entity.GetID()); // Update label

        if (!ImGui::IsItemActive() || pressEnter) // Quit
            editSelectedName = false;
    }
    else
    {
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
        if (isSelectedEntity) nodeFlags |= ImGuiTreeNodeFlags_Selected;

        treeOpen = ImGui::TreeNodeEx(label.c_str(), nodeFlags);

        if (ImGui::IsItemClicked() || ImGui::IsItemToggledOpen())
        {
            selected = &io_entity;
            selectedLabel = label;
        }

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            ImGui::SetDragDropPayload("HIERARCHY_ENTITY", selected, sizeof(Game::Entity*));
            ImGui::Text("Drop on entity to set parent");
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
            {
                Game::Entity* child = static_cast<Game::Entity*>(payload->Data);
                if (child)
                {
                    io_entityManager.UnsetEntityParent(*selected);
                    io_entityManager.SetEntityParent(*selected, io_entity);
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::TableSetColumnIndex(1);

        if (ImGui::Button("X"))
        {
            selected = nullptr;
            removeID = io_entity.GetID();
        }
    }

    ImGui::PopID();

    if (!io_entity.childs.empty())
    {
        ImGui::TableSetColumnIndex(0);
        if (treeOpen)
            RenderEntitiesHierarchy(io_entityManager, io_entity.childs);
    }

    if (treeOpen)
        ImGui::TreePop();
}