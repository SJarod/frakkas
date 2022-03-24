//
// Created by flori on 3/17/2022.
//

#pragma once


namespace Game
{
    class Entity;

    class Component {
    public:
        Component() = default;
        virtual ~Component() = default;

        /**
         * @summary Called when the owner is included into the engine's EntityManager,
         * so it is called once, and beware that other entities may not be set at this moment.
         */
        virtual void Start() = 0;
        /**
         * @summary Called every frame in the game loop if component enabled.
         * The common function to write game script.
         */
        virtual void Update() = 0;

        /**
         * Called every time the component is enabled (no longer disabled).
         */
        virtual void OnEnable() = 0;
        /**
         * Called every time the component is disabled (no longer enabled).
         */
        virtual void OnDisable() = 0;
        /**
         * Called when component is removed, or when owner is deleted
         */
        virtual void OnDestroy() = 0;

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
        bool IsEnabled() const;

        /**
         * Set or replace the current entity which hold the component. You can unset the owner by sending nullptr.
         * @param owner an entity floating pointer, should be store somewhere else as smart pointer.
         */
        void SetOwner(Entity* owner);

    protected:

        Entity* owner = nullptr;
        bool enabled = true;

    };
}



