#pragma once

namespace Game
{
    struct Transform;
}

namespace Helpers
{
    /**
     * @brief ImGui editing function. Allows the player to change transform parameters.
     * @param io_transform the transform to edit
     */
    void EditTransform(Game::Transform& io_transform);
}
