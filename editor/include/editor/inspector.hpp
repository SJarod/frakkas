#pragma once

#include <string>
#include <unordered_map>

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
        void OnImGuiRender(Game::Entity* io_selectedEntity, Game::EntityManager& io_entityManager, int& i_gizmoType);
    };
}
