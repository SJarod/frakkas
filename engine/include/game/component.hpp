#pragma once

#include <string>
#include <utility>
#include <vector>

#include "utils/properties.hpp"
#include "game/reflection/data_descriptor.hpp"





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
         * @return A registry array which store all Component metadata.
         */
        static std::vector<ClassMetaData*>& GetRegistry();

        /**
         * @return an ID that identify the component. Useful to find a specific component in a bunch of another components.
         */
        virtual const std::string& GetID() const = 0;
        
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
         * @brief an abstract function, every inheretid components should define this function with KK_GENERATE_COMPONENT
         * @return the meta data of the class, in other words the list of member variable readable for the engine.
         */
        virtual const ClassMetaData& GetMetaData() const = 0;

        /**
         * @Summary Setup component parameters from input file.
         * @param i_file the opened input file.
         */
        virtual void Read(std::ifstream& i_file, const Resources::Serializer& i_serializer) {};

        /**
         * @Summary SaveScene the component parameters in scene text format.
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
}
