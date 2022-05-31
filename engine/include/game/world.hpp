#pragma once

class Engine;

namespace Game
{
    using InputsMode = unsigned int;

    /**
     * @Summary World class is a kind of gate for entities to access Engine features as inputs listening or scene management.
     * It also has some default values as default scene.
     */
    class World
    {
    public:
        static constexpr InputsMode InputsMode_Game = 1 << 0;
        static constexpr InputsMode InputsMode_UI = 1 << 1;

        explicit World(Engine& i_engine);
        ~World() = default;

        std::string defaultScenePath = "game/assets/Scenes/menu.kk";

        /**
         * @brief Setup the inputs mode to enable UI or Game
         * @param i_flag InputsMode flag to describe the current inputs mode.
         */
        static void SetInputsMode(InputsMode i_flag);

        static InputsMode GetInputsMode();

        /**
         * @brief Load a new scene.
         * @param i_scenePath The path to the scene asset.
         */
        static void LoadScene(const std::string& i_scenePath);

        static void ReloadScene();

    private:
        static InputsMode inputsMode;
        static Engine* engine;
    };
}
