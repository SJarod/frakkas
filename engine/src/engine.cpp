#include <iostream>

#include <glad/glad.h>
#include <SDL.h>
#include <backends/imgui_impl_sdl.h>
#include <Tracy.hpp>

#include "maths.hpp"

#include "game/entity.hpp"
#include "game/drawable.hpp"
#include "game/camera_component.hpp"
#include "game/entity_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"

#include "engine.hpp"

Engine::Engine()
{
    InitSDL();

    renderer = std::make_unique<Renderer::LowLevel::LowRenderer>("basic");
    editorFBO = std::make_unique<Renderer::LowLevel::Framebuffer>(1920, 1080);
    gameFBO = std::make_unique<Renderer::LowLevel::Framebuffer>(1920, 1080);

    CreateTestEntities();
}

Engine::~Engine()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        std::cout << "ERROR: Can't init SDL - " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL4 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

    if (!window)
    {
        std::cout << "ERROR: Can't create window - " << SDL_GetError() << std::endl;
        exit(1);
    }

    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

    // Load GL extensions using glad
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize the OpenGL context." << std::endl;
        exit(1);
    }
}

void Engine::CreateTestEntities()
{
// Create 5 entities for example
    for (int i = 0; i < 5; i++)
    {
        Game::Entity* entity = entityManager.CreateEntity();
        entity->transform.position = Vector3(i * 2.f, 0.f, 0.f);
        entity->transform.scale = Vector3(i * 0.2f + 0.2f, i * 0.2f + 0.2f, i * 0.2f + 0.2f);

        if( i >= 1)
        {
            entity->AddComponent(std::make_unique<Game::Drawable>());
            auto& model = entity->GetComponent<Game::Drawable>("drawable")->model;
            model.AddMeshesFromFile("game/assets/bp.fbx", "game/assets/bp.jpg", false);
            model.transform.scale = Vector3(0.01f, 0.01f, 0.01f);
        }
        else
        {
            entity->AddComponent(std::make_unique<Game::CameraComponent>());
        }
    }
}

void Engine::Run()
{
    bool running = true;
    while(running)
    {
        InputsPollEvent();
        /// NEW FRAME
        timeManager.NewFrame();

        renderer->BeginFrame(*editorFBO);
        entityManager.Render(*renderer, editorFBO->aspectRatio);

        renderer->BeginFrame(*gameFBO);
        entityManager.UpdateAndRender(*renderer, gameFBO->aspectRatio);

        renderer->EndFrame();

        for (const UpdateEvent& updateEvent : updateEventsHandler)
            updateEvent();

        /// ENDFRAME

        running  = !inputs.quit;
        SDL_GL_SwapWindow(window);
    }
}

void Engine::InputsPollEvent()
{
    SDL_Event evt;
    /// INPTUS EVENT
    while (SDL_PollEvent(&evt))
    {
        ImGui_ImplSDL2_ProcessEvent(&evt);
        switch(evt.type)
        {
            case SDL_QUIT:
                inputs.quit = true;
                break;
            case SDL_KEYDOWN:
                switch(evt.key.keysym.sym)
                {
                    case SDLK_RIGHT: inputs.horizontal = 1.f; break;
                    case SDLK_LEFT: inputs.horizontal = -1.f; break;
                    case SDLK_UP: inputs.vertical = 1.f; break;
                    case SDLK_DOWN: inputs.vertical = -1.f; break;
                    case SDLK_SPACE: inputs.depth = 1.f; break;
                    case SDLK_LCTRL: inputs.depth = -1.f; break;
                    default: break;
                }
                break;
            case SDL_KEYUP:
                switch(evt.key.keysym.sym)
                {
                    case SDLK_RIGHT: inputs.horizontal = 0.f; break;
                    case SDLK_LEFT: inputs.horizontal = 0.f; break;
                    case SDLK_UP: inputs.vertical = 0.f; break;
                    case SDLK_DOWN: inputs.vertical = 0.f; break;
                    case SDLK_SPACE: inputs.depth = 0.f; break;
                    case SDLK_LCTRL: inputs.depth = 0.f; break;
                    default: break;
                }
                break;
            case SDL_MOUSEMOTION:
                break;
            default:
                break;
        }
    }
}


