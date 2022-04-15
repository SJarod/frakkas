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

        std::vector<std::unique_ptr<Component>> components;
        Transform transform;

        /**
         * Push back the input unique component in components array. You must use std::move().
         * Don't add a same component twice, updating process won't be correct.
         * @param comp the component to add. The shared_ptr instance will be copied.
         */
        void AddComponent(std::unique_ptr<Component> comp);

        template<typename T>
        T* AddComponent();

        /**
         * @brief Find a component by its id
         * @tparam T the class type of the component
         * @return the pointer of first T-type component if found, else nullptr
         */
        template<typename T>
        T* GetComponent();

        /**
         * @brief Find all components with the same id
         * @tparam T the class type of the components
         * @return all T-type components in a list, list is empty if not found
         */
        template<typename T>
        std::vector<T*> GetComponents();

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
T* Game::Entity::AddComponent()
{
    components.emplace_back(std::make_unique<T>());
    components.back()->owner = this;
    return reinterpret_cast<T*>(components.back().get());
}

template<typename T>
T* Game::Entity::GetComponent() 
{
    std::vector<std::unique_ptr<Component>>::iterator it;
    it = std::find_if(components.begin(), components.end(), [&](const std::unique_ptr<Component>& c){
        return c->GetID() == T::id;
    });

    if (it != components.end())
    {
        if (T* comp = reinterpret_cast<T*>(it->get()))
            return comp;
    }

    return nullptr;
}

template<typename T>
std::vector<T*> Game::Entity::GetComponents()
{
    std::vector<std::unique_ptr<T>> comps;
    for(std::vector<std::unique_ptr<Component>>::iterator it = components.begin(); it != components.end(); it++)
    {
        if (it->get()->GetID() == T::id)
        {
            if (T* comp = reinterpret_cast<T*>(it->get()))
                comps.emplace_back(comp);
        }
    }

    return comps;
}
