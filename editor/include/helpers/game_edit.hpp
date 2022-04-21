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
     * @brief ImGui editing function. Allows the player to change transform parameters.
     * @param io_transform the transform to edit
     */
    void EditTransform(Game::Transform& io_transform);

    /**
     * @brief ImGui editing function. Allows the player to change entity parameters.
     * @param io_entity the entity to edit
     */
    void EditEntity(Game::Entity& io_entity);

    /**
     * @brief ImGui editing function. Allows the player to change camera parameters.
     * @param io_camera the camera to edit
     */
    void EditCamera(Renderer::LowLevel::Camera& io_camera);

    /**
     * @brief ImGui editing function. Allows the player to change light parameters.
     * @param io_light the light to edit
     */
    void EditLight(Renderer::Light& io_light);

    /**
    * @brief ImGui editing function. Allows the player to change sound parameters.
    * @param io_sound the sound to edit
    */
    void EditSound(Resources::Sound& io_sound);

    /**
     * @brief ImGui editing function. Allows the player to change reflected component fields.
     * @param io_component The component itself, cast in an unsigned char* pointer for simple memory work.
     * @param metaData The metaData of the component, which means all the reflected informations to use io_component correctly.
     */
    void EditComponentMetaData(unsigned char* io_component, const ClassMetaData& metaData, Property<bool>& io_enabled);
}