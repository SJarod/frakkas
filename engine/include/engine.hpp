#pragma once

#include <memory>
#include <list>
#include <functional>

#include <SDL.h>

#include "game/time_manager.hpp"

namespace Renderer::LowLevel
{
    class LowRenderer;
    class Framebuffer;
}

using UpdateEvent = std::function<void()>;

namespace Game
{
    class EntityManager;
}

struct Inputs
{
    bool quit = false;
    float horizontal = 0.f;
    float vertical = 0.f;
    float depth = 0.f;
};

class Engine
{
public:
    Engine();
    ~Engine();

    Game::EntityManager entityManager;

    std::unique_ptr<Renderer::LowLevel::LowRenderer> renderer;
    std::unique_ptr<Renderer::LowLevel::Framebuffer> editorFBO;
    std::unique_ptr<Renderer::LowLevel::Framebuffer> gameFBO;

    /**
     * Engine main loop. Manage frames and Render entities.
     * Call all UpdateEvent attached to UpdateEventHandler.
     */
    void Run();

    std::list<UpdateEvent> updateEventsHandler;

private:

    Game::Time timeManager;
    Inputs inputs;

    SDL_Window* window = nullptr;
    SDL_GLContext glContext;

    /**
     * Init SDL parameters, link to OpenGL and create a window.
     */
    void InitSDL();
    /**
     * Create entities in hard-code to test our project
     */
    void CreateTestEntities();

    /**
     * Listen to SDL inputs Event, and set our Inputs structure to get inputs informations
     */
    void InputsPollEvent();

};