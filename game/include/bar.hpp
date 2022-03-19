//
// Created by flori on 3/17/2022.
//

#pragma once

#include "component.hpp"

class Bar : public Component {

public:
    Bar() = default;
    ~Bar() = default;

    void Start() override;
    void Update() override;

    void OnEnable() override {};
    void OnDisable() override {};
    void OnDestroy() override {};
};


