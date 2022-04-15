#pragma once

namespace Game
{
    struct Transform;
    class Component;
}

namespace Renderer
{
    class Light;
}

namespace Helpers
{
    /**
     * @Summary ImGui editing function. Allows the player to change transform parameters.
     * @param io_transform the transform to edit
     */
    void EditTransform(Game::Transform& io_transform);

    /**
    * @Summary ImGui editing function. Allows the player to change light parameters.
    */
    void EditLight(Renderer::Light& io_light);

    bool ComponentBeginEdit(Game::Component* comp);
    void ComponentEndEdit();
}
