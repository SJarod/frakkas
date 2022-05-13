#pragma once

#include <imgui.h>

#include "game/entity.hpp"
#include "game/entity_manager.hpp"
#include "game/lowcomponent/sound_component.hpp"
#include "game/lowcomponent/static_draw.hpp"

#include "renderer/graph.hpp"

#include "helpers/string_helpers.hpp"

namespace Helpers
{

    /**
     * @brief Read path extension to add a component to the output entity, or load a new scene.
     * @param i_path The input file path dropped.
     * @param o_entity The output entity to receive new component
     * @param io_graph The graph to handle .kk drop. Optional.
     * @return true if a resource has been load, false if nothing happens.
     */
    inline bool ReadPath(const std::filesystem::path& i_path, Game::Entity& o_entity, Renderer::Graph* io_graph = nullptr)
    {
        if (i_path.extension() == ".wav" || i_path.extension() == ".mp3")
        {
            auto soundComp = o_entity.AddComponent<Game::SoundComponent>();
            soundComp->sound.SetSound(i_path.string());
            return true;;
        }
        else if (i_path.extension() == ".obj" || i_path.extension() == ".fbx" || i_path.extension() == ".gltf")
        {
            auto staticDraw = o_entity.AddComponent<Game::StaticDraw>();
            staticDraw->model.SetMeshFromFile(i_path.string());
            return true;
        }
        else if (i_path.extension() == ".cpp" || i_path.extension() == ".hpp")
        {
            std::filesystem::path fileName = i_path.filename().replace_extension();
            ClassMetaData* md = Game::Component::FindMetaData(Helpers::SnakeCaseToCamelCase(fileName.string()));
            if (md)
            {
                o_entity.AddComponent(std::unique_ptr<Game::Component>(md->constructor()));
                return true;
            }
        }
        else if (io_graph && i_path.extension() == ".kk") // Handle .KK Loading if graph is send
        {
            io_graph->LoadScene(i_path.string());
            return true;
        }

        return false;
    }

    /**
     * @brief Call ImGui "BeginDragDropTarget" to enable the last ImGui item as a resource path receiver.
     * @param o_entity The entity to add component if the data drop is a resource.
     * @param io_graph The graph to handle .kk drop. Optional.
     * @return true if a resource has been load, false if nothing happens.
     */
	inline bool DragDropTargetComponent(Game::Entity& o_entity, Renderer::Graph* io_graph = nullptr)
	{
        bool dropped = false;
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_BROWSER_ITEM"))
            {
                std::filesystem::path path = *static_cast<std::filesystem::path*>(payload->Data);

                dropped = ReadPath(path, o_entity, io_graph);
            }
            ImGui::EndDragDropTarget();
        }
        return dropped;
	}

    /**
     * @brief Call ImGui "BeginDragDropTarget" to enable the last ImGui item as a resource path receiver.
     * @param o_entityManager The entity manager to add new entity with the component build by the resource dropped.
     * @param io_graph The graph to handle .kk drop. Optional.
     * @return true if a resource has been load, false if nothing happens.
     */
    inline bool DragDropTargetComponent(Game::EntityManager& o_entityManager, Renderer::Graph* io_graph = nullptr)
    {
        bool dropped = false;
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_BROWSER_ITEM"))
            {
                std::filesystem::path path = *static_cast<std::filesystem::path*>(payload->Data);

                Game::Entity* entity = o_entityManager.CreateEntity();

                dropped = ReadPath(path, *entity, io_graph);

                if (!dropped)
                    o_entityManager.RemoveEntityAt(entity->GetID());
            }
            ImGui::EndDragDropTarget();
        }
        return dropped;
    }

}
