//
// Created by flori on 3/17/2022.
//

#pragma once

#include "properties.hpp"

#include "resources/serializer.hpp"

namespace Game
{
    class Entity;

    class Component {
    public:
        Component(const std::string& id) : id(id) {};
        virtual ~Component() = default;

        std::string id = "None";

        /**
         * @summary Called when the owner is included into the engine's EntityManager,
         * so it is called once, and beware that other entities may not be set at this moment.
         */
        virtual void Start() {};
        /**
         * @summary Called every frame in the game loop if component enabled.
         * The common function to write game script.
         */
        virtual void Update() {};

        /**
         * Called every time the component is enabled (no longer disabled).
         */
        virtual void OnEnable() {};
        /**
         * Called every time the component is disabled (no longer enabled).
         */
        virtual void OnDisable() {};
        /**
         * Called when component is removed, or when owner is deleted
         */
        virtual void OnDestroy() {};

        /**
         * Enable/Activate the component if it is disabled. Call OnEnable() too.
         */
        void Enable();
        /**
         * Disable/Deactivate the component if it is enabled. Call OnEnable() too.
         */
        void Disable();

        /**
         * @return the enable state of the component
         */
        [[nodiscard]] bool IsEnabled() const;

        /**
         * ImGui editing function. Set which parameters can be modified in run time.
         */
        virtual void Edit() {};

        /**
         * Setup component parameters from input file.
         * @param i_file the opened input file.
         */
        virtual void Read(std::ifstream& i_file, const Resources::Serializer& i_serializer) {};

        /**
         * Write the component parameters in scene text format.
         * @param o_file teh opened output file.
         */
        virtual void Write(std::ofstream& o_file, const Resources::Serializer& i_serializer) const {};


        WRITEONLY_PROPERTY(Entity*, owner);
        SET(owner)
        {
            SetOwner(value);
        }


    protected:
        /**
         * Set or replace the current entity which hold the component. You can unset the owner by sending nullptr.
         * @param owner an entity floating pointer, should be store somewhere else as smart pointer.
         */
        virtual void SetOwner(Entity* owner);

        bool enabled = true;

    private:
        Entity* _owner = nullptr;

    };
}



