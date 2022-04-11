#pragma once

#include <string>

#include "properties.hpp"

namespace Resources
{
    class Serializer;
}

namespace Game
{
    class Entity;

    class Component
    {
    public:
        Component();
        virtual ~Component() = default;

        Property<bool> enabled;
        Property<Entity*> owner;
        
        /**
         * @return an ID that identify the component. Useful to find a specific component in a bunch of another components.
         */
        virtual const std::string& GetID() const { return "None"; };
        
        /**
         * @summary Called when the owner is included into the engine's EntityManager,
         * so it is called once even if component should be disabled,
         * and beware that other entities may not be set at this moment.
         */
        virtual void Start() {};

        /**
         * @Summary Called every frame in the game loop if component enabled.
         * The common function to write game script.
         */
        virtual void Update() {};

        /**
         * Called when component is removed, or when owner is deleted.
         * Called even if component is disabled.
         */
        virtual void OnDestroy(){};

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


    protected:

        /**
         * @Summary Set or replace the current entity which hold the component. You can unset the entity by sending nullptr.
         * @param entity an entity floating pointer, should be store somewhere else as smart pointer.
         */
        virtual void SetOwner(Entity* entity);

        /**
         * @brief Called when change enabled state. Could be consider as a Start() but can be call many times.
         * Not called when entity started
         */
        virtual void OnEnable() {};
        /**
         * @brief Call when change enabled state. Could be consider as a OnDestroy() but can be call many times.
         * Not called when entity destroyed
         */
        virtual void OnDisable() {};
    };

    /**
     * BaseComponent is the base of every game's component. It simply inherits from Component
     * and add a static string id. Thanks to that, every game's component will have an id and we will be able to identify them properly.
     */
    template<const char* TComponentID>
    class BaseComponent : public Component
    {
    public:
        static std::string id;

        const std::string& GetID() const override { return this->id; }
    };
}

template<const char* TComponentID>
std::string Game::BaseComponent<TComponentID>::id = std::string(TComponentID);
