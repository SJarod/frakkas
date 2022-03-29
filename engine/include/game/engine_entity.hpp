//
// Created by flori on 3/23/2022.
//

#pragma once

#include "game/entity.hpp"
#include "renderer/model.hpp"
#include "maths.hpp"
#include "game/entity.hpp"

namespace Game
{
    class Transform;

    class EngineEntity : public Entity
    {
    public:
        EngineEntity();

        [[nodiscard]] Renderer::Model& GetModel();

        Game::Transform& GetModelTransform();

        void Edit() override;

    private:
        Renderer::Model model;
    };
}
