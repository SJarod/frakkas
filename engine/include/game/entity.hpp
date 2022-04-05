#pragma once

#include <vector>
#include <memory>

#include "transform.hpp"
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

        // TODO: Make unique?
        std::vector<std::unique_ptr<Component>> components;
        Transform transform;

        /**
         * Push back the input unique component in components array. You must use std::move().
         * Don't add a same component twice, updating process won't be correct.
         * @param comp the component to add. The shared_ptr instance will be copied.
         */
        void AddComponent(std::unique_ptr<Component> comp);

        /**
         * @brief Find a component by its name id
         * @tparam T the class type of the component
         * @param id the name id of the component
         * @return the first T-type component if found, else nullptr
         */
        template<typename T>
        T* GetComponent(const std::string& id);

        /**
         * @brief Find all components with the same name id
         * @tparam T the class type of the components
         * @param id the name id of the components
         * @return all T-type components in a list, list is empty if not found
         */
        template<typename T>
        std::vector<T*> GetComponents(const std::string& id);

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
    };
}

template<typename T>
T* Game::Entity::GetComponent(const std::string &id)
{
    std::vector<std::unique_ptr<Component>>::iterator it;
    it = std::find_if(components.begin(), components.end(), [&id](const std::unique_ptr<Component>& c){
        return c->id == id;
    });

    if (it != components.end())
    {
        if (T* comp = reinterpret_cast<T*>(it->get()))
            return comp;
    }

    return nullptr;
}

template<typename T>
std::vector<T*> Game::Entity::GetComponents(const std::string &id)
{
    std::vector<std::unique_ptr<T>> comps;
    for(std::vector<std::unique_ptr<Component>>::iterator it = components.begin(); it != components.end(); it++)
    {
        if (it->get()->id == id)
        {
            if (T* comp = reinterpret_cast<T*>(it->get()))
                comps.push_back(comp);
        }
    }

    return comps;
}