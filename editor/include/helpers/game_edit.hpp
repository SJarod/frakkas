#pragma once

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
    void Edit(Game::Entity& io_entity);

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
     */
    void Edit(unsigned char* io_component, const ClassMetaData& io_metaData, bool& io_enabled);
}
