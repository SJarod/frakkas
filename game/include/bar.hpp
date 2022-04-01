#pragma once

#include "game/component.hpp"

namespace Game
{
    class Bar : public Component
    {
    public:
        Bar() = default;
        ~Bar() = default;

        void Start() override;
        void Update() override;

        void OnEnable() override {};
        void OnDisable() override {};
        void OnDestroy() override {};
    };
}