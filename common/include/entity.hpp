//
// Created by flori on 3/17/2022.
//

#pragma once

#include <vector>
#include <memory>

#include "component.hpp"

class Entity
{
public:
    Entity() = default;
    ~Entity();

    /**
     * @summary Called when the entity is included into the Engine's EntityManager,
     * so it is called once, and beware that other entities may not be set at this moment.
     * Process the Start function of all entity's components
     */
    void Start();
    /**
     * @summary Called every frame in the game loop.
     * Process the Update function of all entity's components.
     */
    void Update();

    /**
     * Push back the input component in components array.
     * Don't add a same component twice, updating process won't be correct.
     * @param comp the component to add. The shared_ptr instance will be copied.
     */
    void AddComponent(const std::shared_ptr<Component>& comp);

private:

    std::vector<std::shared_ptr<Component>> components;
};