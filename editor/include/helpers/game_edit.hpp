#pragma once

#include <string>
#include <unordered_map>

#include "log.hpp"
#include "engine.hpp"

#include "game/inputs_manager.hpp"

#include "editor/editor_render.hpp"

namespace Game
{
	class Entity;
	struct Transform;
}

namespace Renderer
{
    class Light;
    namespace LowLevel
    {
        class Camera;
    }
}

namespace Resources
{
    class Sound;
}

struct ClassMetaData;

namespace Helpers
{
     /**
     * @brief Call ImGui::DragScalarN and activate cursor game mode for comfortability.
     * @tparam TScalarType Type of the scalar : int or float
     * @param i_label The variable name to print on editor.
     * @param io_data The pointer to the scalar array.
     * @param i_count The number of scalar in the array.
     * @param i_speed The quantity step that changes the scalar value when dragging.
     * @param i_min The minimum value to clamp at.
     * @param i_max The maximum value to clamp at.
     * @return true if user edit the scalar, false if nothing happened.
     */
     template<typename TScalarType> requires std::is_scalar_v<TScalarType>
    bool DragScalar(const std::string_view& i_label, TScalarType* io_data, int i_count, float i_speed = 0.5f, TScalarType i_min = std::numeric_limits<TScalarType>::lowest(), TScalarType i_max = std::numeric_limits<TScalarType>::max())
    {
        ImGuiDataType_ dataType = ImGuiDataType_Float;
        if constexpr (std::is_same_v<TScalarType, int>)
            dataType = ImGuiDataType_S32;
        else if constexpr (std::is_floating_point_v<TScalarType>)
            dataType = ImGuiDataType_Float;
        else
        {
                Log::Warning("Intended to drag-edit a wrong type.");
                return false;
        }

        if (ImGui::DragScalarN(i_label.data(), dataType, io_data, i_count, i_speed, &i_min, &i_max))
        {
            if (!Editor::EditorRender::isEditingDrag)
            {
                Engine::SetCursorGameMode(true);
                Editor::EditorRender::isEditingDrag = true;
                Editor::EditorRender::mouseLockPosition = Game::Inputs::GetMousePosition();
            }
            return true;
        }
        return false;
    }

	/**
    * @brief ImGui editing function. Allows the player to edit a string variable.
     * @param io_string The string to display and edit.
     * @param i_label The variable name of the string. Called 'label' for ImGui field.
     */
    void Edit(std::string& io_string, const char* i_label);
    /**
     * @brief ImGui editing function. Allows the player to change transform parameters.
     * @param io_transform the transform to edit
     */
    void Edit(Game::Transform& io_transform);

    /**
     * @brief ImGui editing function. Allows the player to change entity parameters.
     * @param io_entity the entity to edit
     */
    void Edit(Game::Entity& io_entity, const std::unordered_map<const char*, Game::Transform*>& entityTransforms, int& i_gizmoType);

    /**
     * @brief ImGui editing function. Allows the player to change camera parameters.
     * @param io_camera the camera to edit
     */
    void Edit(Renderer::LowLevel::Camera& io_camera);

    /**
     * @brief ImGui editing function. Allows the player to change light parameters.
     * @param io_light the light to edit
     */
    void Edit(Renderer::Light& io_light);

    /**
    * @brief ImGui editing function. Allows the player to change sound parameters.
    * @param io_sound the sound to edit
    */
    void Edit(Resources::Sound& io_sound);

    /**
     * @brief ImGui editing function. Allows the player to change reflected component fields.
     * @param io_component The component itself, cast in an unsigned char* pointer for simple memory work.
     * @param io_metaData The metaData of the component, which means all the reflected informations to use io_component correctly.
     * @return false if the component should be removed.
     */
    bool Edit(unsigned char* io_component, const ClassMetaData& io_metaData, bool& io_enabled);
}
