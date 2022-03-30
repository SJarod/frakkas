//
// Created by m.mehalin on 16/03/2022.
//

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
        void OnImGuiRender(Game::EntityManager &entityManager);

        Game::Entity* selected = nullptr;
        std::string selectedLabel;
    };
}
