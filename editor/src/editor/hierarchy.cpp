#include <imgui.h>
#include <string>

#include "game/lowcomponent/static_draw.hpp"
#include "game/lowcomponent/camera.hpp"
#include "game/lowcomponent/collider/box_collider.hpp"
#include "game/lowcomponent/collider/sphere_collider.hpp"
#include "game/entity_manager.hpp"

#include "resources/mesh.hpp"

#include "game/inputs_manager.hpp"

#include "helpers/game_edit.hpp"

#include "utils/dragdrop_constants.hpp"

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

    bool isWindowHovered = ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered();

    OpenPopupAndUnselect(isWindowHovered);

    // reset remove id
    removeID = -1;

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

    // Create a dummy item to enable dragDropTarget
    ImGui::InvisibleButton("InvisibleDropTarget", ImVec2(500, 500));
    ParentUnsetDragDropTarget(io_entityManager);
    OpenPopupAndUnselect(ImGui::IsItemHovered());

    // Remove an entity if remove ID had been modify
    if (removeID != -1)
        io_entityManager.RemoveEntityAt(removeID);

    ImGui::End();

    if(openAddEntityPopup)
    {
        ImGui::OpenPopup("ADDENTITY");
    }

    AddEntityPopup(io_entityManager);
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
    bool addEntityPopup = false;

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

        SetupEntityInteraction(io_entityManager, io_entity, label);

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

void Hierarchy::SetupEntityInteraction(Game::EntityManager& io_entityManager, Game::Entity& io_entity, std::string& io_label)
{
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemToggledOpen())
    {
        selected = &io_entity;
        selectedLabel = io_label;
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
        selected = &io_entity;
        selectedLabel = io_label;
        openAddEntityPopup = true;
        selectedWhenAdd = selected;
    }

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        ImGui::SetDragDropPayload(Utils::EntityDragDropID, selected, sizeof(Game::Entity*));
        ImGui::Text("Drop on entity to set parent");
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Utils::EntityDragDropID))
        {
            Game::Entity* child = static_cast<Game::Entity*>(payload->Data);
            if (child)
            {
                auto it = std::find(io_entity.childs.begin(), io_entity.childs.end(), selected);
                // Set parent only if selected is not already a child
                if (it == io_entity.childs.end())
                {
                    io_entityManager.UnsetEntityParent(*selected);
                    io_entityManager.SetEntityParent(*selected, io_entity);
                }
                else
                    io_entityManager.UnsetEntityParent(*selected);
            }
        }
        ImGui::EndDragDropTarget();
    }

}

void Hierarchy::ParentUnsetDragDropTarget(Game::EntityManager& io_entityManager) const
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Utils::EntityDragDropID))
        {
            Game::Entity* child = static_cast<Game::Entity*>(payload->Data);
            if (child)
            {
                io_entityManager.UnsetEntityParent(*selected);
            }
        }
        ImGui::EndDragDropTarget();
    }
}

void Hierarchy::AddEntityPopup(Game::EntityManager& io_entityManager)
{
    if (ImGui::BeginPopup("ADDENTITY", ImGuiWindowFlags_AlwaysAutoResize))
    {
        openAddEntityPopup = false;

        ImGui::Text("Add entity");
        ImGui::Separator();

        Game::Entity* entity = nullptr;
        if (ImGui::Selectable("Empty"))
        {
            entity = io_entityManager.CreateEntity();
        }
        else if (ImGui::Selectable("Cube mesh"))
        {
            entity = io_entityManager.CreateEntity();
            entity->name = "Cube_" + std::to_string(entity->GetID());
            auto drawable = entity->AddComponent<Game::StaticDraw>();
            drawable->model.SetMeshFromFile(Resources::Mesh::cubeMesh);
            drawable->model.SetTexture("game/assets/gold.jpg", true);
        }
        else if (ImGui::Selectable("Sphere mesh"))
        {
            entity = io_entityManager.CreateEntity();
            entity->name = "Sphere_" + std::to_string(entity->GetID());
            auto drawable = entity->AddComponent<Game::StaticDraw>();
            drawable->model.SetMeshFromFile(Resources::Mesh::sphereMesh);
            drawable->model.SetTexture("game/assets/gold.jpg", true);
        }
        else if (ImGui::Selectable("Box collider"))
        {
            entity = io_entityManager.CreateEntity();
            entity->name = "BoxCollider_" + std::to_string(entity->GetID());
            entity->AddComponent<Game::BoxCollider>();
        }
        else if (ImGui::Selectable("Sphere collider"))
        {
            entity = io_entityManager.CreateEntity();
            entity->name = "SphereCollider_" + std::to_string(entity->GetID());
            entity->AddComponent<Game::SphereCollider>();
        }
        else if (ImGui::Selectable("Camera"))
        {
            entity = io_entityManager.CreateEntity();
            entity->name = "Camera_" + std::to_string(entity->GetID());
            entity->AddComponent<Game::Camera>();
        }

        if (entity && selectedWhenAdd)
        {
            io_entityManager.SetEntityParent(*entity, *selectedWhenAdd);
            selectedWhenAdd = nullptr;
        }

        selected = entity;

        ImGui::EndPopup();
    }
}

void Hierarchy::OpenPopupAndUnselect(bool i_condition)
{
    if (i_condition && Game::Inputs::IsPressed(Game::EButton::MOUSE_RIGHT))
    {
        openAddEntityPopup = true;
        selectedWhenAdd = selected = nullptr;
    }

    if (i_condition && Game::Inputs::IsPressed(Game::EButton::MOUSE_LEFT))
        selectedWhenAdd = selected = nullptr;
}