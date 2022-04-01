#pragma once

#include "properties.hpp"
#include "resources/serializer.hpp"


namespace Game
{
    class Entity;

    class Component
    {
    public:
        explicit Component(const std::string& id);
        virtual ~Component() = default;

        std::string id = "None";

        /**
         * @Summary Called when the owner is included into the engine's EntityManager,
         * so it is called once, and beware that other entities may not be set at this moment.
         */
        virtual void Start() {};

        /**
         * @Summary Called every frame in the game loop if component enabled.
         * The common function to write game script.
         */
        virtual void Update() {};

        /**
         * @Summary Called every time the component is enabled (no longer disabled).
         */
        virtual void OnEnable() {};

        /**
         * @Summary Called every time the component is disabled (no longer enabled).
         */
        virtual void OnDisable() {};

        /**
         * @Summary Called when component is removed, or when owner is deleted
         */
        virtual void OnDestroy() {};

        /**
         * @Summary Enable/Activate the component if it is disabled. Call OnEnable() too.
         */
        void Enable();

        /**
         * @Summary Disable/Deactivate the component if it is enabled. Call OnEnable() too.
         */
        void Disable();

        /**
         * @return the enable state of the component
         */
        [[nodiscard]] bool IsEnabled() const;

        /**
         * @Summary ImGui editing function. Set which parameters can be modified in run time.
         */
        virtual void Edit() {};

        /**
         * @Summary Setup component parameters from input file.
         * @param i_file the opened input file.
         */
        virtual void Read(std::ifstream& i_file, const Resources::Serializer& i_serializer) {};

        /**
         * @Summary Write the component parameters in scene text format.
         * @param o_file teh opened output file.
         */
        virtual void Write(std::ofstream& o_file, const Resources::Serializer& i_serializer) const {};

        Property<Entity*> owner;

    protected:
        /**
         * @Summary Set or replace the current entity which hold the component. You can unset the owner by sending nullptr.
         * @param owner an entity floating pointer, should be store somewhere else as smart pointer.
         */
        virtual void SetOwner(Entity* owner);

        bool enabled = true;
    };
}
