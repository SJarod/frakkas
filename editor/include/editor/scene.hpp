#pragma once

#include <ImGuizmo.h>

#include "maths.hpp"

namespace Renderer::LowLevel
{
    class Framebuffer;
    class Camera;
}

namespace Game
{
    class Entity;
}
typedef void* ImTextureID;

namespace Editor
{
    class Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;

        /**
         * Tell the editor if user is moving in the scene or not.
         */
        bool isMoving = false;
        /**
         * Position where the cursor is locked when player begin moving.
         */
        Vector2 mouseLockPosition;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender(Renderer::LowLevel::Framebuffer& io_fbo, Renderer::LowLevel::Camera& i_camera, Game::Entity* i_selectedEntity, ImGuizmo::OPERATION& i_gizmoOperation);

        /**
         * @summary Listen to mouse right click, and enable/disable movement in editor scene.
         */
        void CheckMouseAction();
    };
}
