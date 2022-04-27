
#include "game/entity.hpp"

namespace Game
{
    /**
     * This class create an Entity with all Game project's components.
     * It is important to call all component constructor once to activate reflection.
     * You should edit the .cpp to add every new components.
     */
    class ComponentRegister
    {
    public:
        ComponentRegister();

    private:
        Entity greedEntity;
    };
}