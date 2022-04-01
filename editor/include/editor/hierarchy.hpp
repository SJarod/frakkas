#pragma once

namespace Game
{
    class Entity;
    class EntityManager;
}

namespace Editor
{
    class Hierarchy
    {
    public:
        Hierarchy() = default;
        ~Hierarchy() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender(Game::EntityManager& i_entityManager);

        Game::Entity* selected = nullptr;
        std::string selectedLabel;
    };
}