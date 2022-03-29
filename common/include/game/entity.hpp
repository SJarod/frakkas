//
// Created by flori on 3/17/2022.
//

#pragma once

#include <vector>
#include <memory>

#include "transform.hpp"
#include "resources/serializer.hpp"
#include "component.hpp"

namespace Resources
{
    class Serializer;
}

namespace Game
{
    class Entity
    {
    public:
        Entity() = default;
        ~Entity();

        Transform transform;

        /**
         * @summary Called when the entity is included into the engine's EntityManager,
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

        /**
         * ImGui editing function. Set which parameters can be modified in run time.
         */
        virtual void Edit();

        /**
         * Setup entity components from input file.
         * @param i_file the opened input file.
         */
        virtual void Read(std::ifstream& i_file, const Resources::Serializer& i_serializer) {};

        /**
         * Write the entity components in scene text format.
         * @param o_file the opened output file.
         */
        virtual void Write(std::ofstream& o_file, const Resources::Serializer& i_serializer) const {};


    protected:

        std::vector<std::shared_ptr<Component>> components;
    };
}
