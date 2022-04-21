#pragma once

#include <memory>
#include <list>
#include <functional>

#include <SDL.h>
#include <miniaudio.h>

#include "game/entity_manager.hpp"
#include "game/time_manager.hpp"
#include "game/inputs_manager.hpp"

namespace Renderer::LowLevel
{
    class LowRenderer;
    class Framebuffer;
}

using UpdateEvent = std::function<void()>;

class Engine
{
public:
    Engine();
    ~Engine();

    Game::EntityManager entityManager;

    std::unique_ptr<Renderer::LowLevel::LowRenderer> renderer;
    std::unique_ptr<Renderer::LowLevel::Framebuffer> editorFBO;
    std::unique_ptr<Renderer::LowLevel::Framebuffer> gameFBO;

    std::list<UpdateEvent> updateEventsHandler;

    static ma_engine soundEngine;

    /**
     * Engine main loop. Manage frames and Render entities.
     * Call all UpdateEvent attached to UpdateEventHandler.
     */
    void Run();

private:
    Game::Time timeManager;
    Game::Inputs inputsManager;

    SDL_Window* window = nullptr;
    SDL_GLContext glContext;

    /**
     * Init SDL parameters, link to OpenGL and create a window.
     */
    void InitSDL();

    /**
    * Init SDL audio parameters.
    */
    void InitMiniaudio();

    /**
     * Create entities in hard-code to test our project
     */
    void CreateTestEntities();
};
