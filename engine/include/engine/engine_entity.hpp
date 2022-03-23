//
// Created by flori on 3/23/2022.
//

#pragma once

#include "entity.hpp"
#include "engine/model.hpp"
#include "maths.hpp"
#include "../../../common/include/entity.hpp"

namespace Physics
{
    class Transform;
}

namespace Engine
{
    class EngineEntity : public Entity
    {
    public:
        [[nodiscard]] const Engine::Model& GetModel() const;

        Physics::Transform& GetTransform();

    private:
        Engine::Model model;
    };
}
