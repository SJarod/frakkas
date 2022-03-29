//
// Created by m.mehalin on 16/03/2022.
//

#pragma once

namespace Game
{
    class EngineEntity;
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

        Game::EngineEntity* selected = nullptr;
        std::string selectedLabel;
    };
}
