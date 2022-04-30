#pragma once

#include <string>
#include <unordered_map>
#include "ImGuizmo.h"

namespace Game
{
    class Entity;
    class EntityManager;
}

namespace Editor
{
    class Inspector
    {
    public:
        Inspector() = default;
        ~Inspector() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender(Game::Entity* io_selectedEntity, ImGuizmo::OPERATION& i_guizmoOperation);
    };
}
