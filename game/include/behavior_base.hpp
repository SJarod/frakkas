//
// Created by flori on 3/17/2022.
//

#pragma once


class BehaviorBase {

public:
    BehaviorBase() = default;
    ~BehaviorBase() = default;

    virtual void Begin();
    virtual void Update();
    virtual void Destroy();

};


