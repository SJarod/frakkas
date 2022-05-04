#pragma once

#include <memory>
#include <list>
#include <functional>

#include <SDL.h>
#include <miniaudio.h>

#include "physic/physic_scene.hpp"

#include "game/entity_manager.hpp"
#include "game/time_manager.hpp"
#include "game/inputs_manager.hpp"

namespace Renderer
{
    class Graph;
    namespace LowLevel
    {
        class LowRenderer;
        class Framebuffer;
        class Camera;
    }
}

using UpdateEvent = std::function<void()>;

class Engine
{
public:
    Engine();
    ~Engine();

    bool gaming = false;
    bool focusOnGaming = false;

    Game::EntityManager entityManager;
    Physic::PhysicScene physicScene;

    std::unique_ptr<Renderer::LowLevel::LowRenderer> renderer;
    std::unique_ptr<Renderer::LowLevel::Framebuffer> editorFBO;
    std::unique_ptr<Renderer::LowLevel::Framebuffer> gameFBO;

    std::unique_ptr<Renderer::Graph> graph;
    
    std::list<UpdateEvent> updateEventsHandler;
    InputsEvent editorInputsEvent;

    static ma_engine soundEngine;

    /**
     * Editor main loop. Manage frames and Render entities.
     * Call all UpdateEvent attached to UpdateEventHandler.
     */
    void RunEditor();
    /**
     * Game main loop. Manage frames, Render and Update entities.
     * Call all UpdateEvent attached to UpdateEventHandler.
     */
    void RunGame();

    /**
     * Change mouse cursor visibility
     * @param i_visibility The visible state you want
     */
    static void SetCursorVisibility(bool i_visibility = true);
    /**
     * Lock cursor position on the window, useful to move the mouse cursor infinitely. Use deltaMotion infos.
     */
    static void SetCursorGameMode(bool i_gameMode);

    /**
     * Set the mouse cursor to the input position. Relative to window space. Left-Up corner is (0, 0).
     * @param i_position
     */
    static void SetCursorPosition(const Vector2& i_position);

    /**
     * @return The editor camera from Graph class. never return nullptr.
     */
    Renderer::LowLevel::Camera* GetEditorGamera() const;
    /**
     * @return The current game camera from Graph class. Can return nullptr.
     */
    Renderer::LowLevel::Camera* GetGameCamera() const;

private:
    Game::Time timeManager;
    Game::Inputs inputsManager;

    static SDL_Window* window;
    SDL_GLContext glContext;

    /**
     * Init SDL parameters, link to OpenGL and create a window.
     */
    void InitSDL();

	/**
	* Begin a new frame in engine main loop. Inform other libraries and setup engine parameters.
	*/
    void BeginFrame();
    /**
    * End the current frame in engine main loop. Inform other libraries and swap window buffer.
    */
    bool EndFrame();

    /**
    * Init SDL audio parameters. 
    */
    void InitMiniaudio();

    /**
     * Create entities in hard-code to test our project
     */
    void CreateTestEntities();
};
