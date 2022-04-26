#include <iostream>

#include <glad/glad.h>
#include <SDL.h>
#include <Tracy.hpp>

#include "maths.hpp"
#include "debug/log.hpp"
#include "game/entity.hpp"
#include "game/lowcomponent/camera_component.hpp"

#include "resources/resources_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"
#include "renderer/graph.hpp"

#include "engine.hpp"

#if 1
// Run on laptop high perf GPU
extern "C"
{
    __declspec(dllexport) int NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

SDL_Window* Engine::window = nullptr;
ma_engine Engine::soundEngine {};

Engine::Engine()
{
    Log::Init();

    InitSDL();
    InitMiniaudio();

    renderer = std::make_unique<Renderer::LowLevel::LowRenderer>("basic");
    editorFBO = std::make_unique<Renderer::LowLevel::Framebuffer>(1920, 1080);
    gameFBO = std::make_unique<Renderer::LowLevel::Framebuffer>(1920, 1080);

    graph = std::make_unique<Renderer::Graph>(&entityManager);
}

Engine::~Engine()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    //ma_engine_stop(&soundEngine);
    ma_engine_uninit(&soundEngine);
}

static void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        return;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            Log::Error("[HIGH] OPENGL: " + std::string(message));
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            Log::Warning("[MEDIUM] OPENGL: " + std::string(message));
            break;
        case GL_DEBUG_SEVERITY_LOW:
            Log::Warning("[LOW] OPENGL: " + std::string(message));
            break;
        default:
            Log::Warning("[UNKNOWN] OPENGL: " + std::string(message));
    }
}

void Engine::InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        Log::Error("SDL: Can't init SDL - " + static_cast<std::string>(SDL_GetError()));
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
    window = SDL_CreateWindow("Frakkas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

    if (!window)
    {
        Log::Error("SDL: Can't create window - " + static_cast<std::string>(SDL_GetError()));
        exit(1);
    }

    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

    // Load GL extensions using glad
    if (!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
    {
        Log::Error("OPENGL: Failed to initialize the OpenGL context");
        exit(1);
    }

    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(DebugCallback, nullptr);
    }
}

void Engine::InitMiniaudio()
{
    if (ma_engine_init(nullptr, &soundEngine) != MA_SUCCESS)
    {
        Log::Error("Failed to initialize audio engine.");
        exit(1);
    }

    ma_engine_start(&soundEngine);
    Log::Info("Successfully start sound engine");
};

void Engine::BeginFrame()
{
    ResourcesManager::PollGPULoad();

    inputsManager.PollEvent(editorInputsEvent);
    timeManager.NewFrame();
}

bool Engine::EndFrame()
{
    SDL_GL_SwapWindow(window);
    FrameMark

    return  Game::Inputs::IsReleased("quit") && !Game::Inputs::quit;
}

void Engine::RunEditor()
{
    bool running = true;
    while(running)
    {
        BeginFrame();

        renderer->BeginFrame(*editorFBO);
        graph->RenderEditor(*renderer, editorFBO->aspectRatio);

        renderer->BeginFrame(*gameFBO);
        if (gaming)
        {
            if (focusOnGaming)
                inputsManager.SetInputsListening(true);

            entityManager.Update();
            graph->RenderGame(*renderer, gameFBO->aspectRatio);
        }
        else
            graph->RenderGame(*renderer, gameFBO->aspectRatio);

        renderer->EndFrame();


        inputsManager.SetInputsListening(true);
        for (const UpdateEvent& updateEvent : updateEventsHandler)
            updateEvent();

        running = EndFrame();
        inputsManager.SetInputsListening(false);
    }
}

void Engine::RunGame()
{
    inputsManager.SetInputsListening(true);
    SetCursorGameMode(true);
    bool running = true;
    while(running)
    {
        BeginFrame();

		entityManager.Update();

        renderer->BeginFrame();
        graph->RenderGame(*renderer, gameFBO->aspectRatio);
		

        renderer->EndFrame();

        for (const UpdateEvent& updateEvent : updateEventsHandler)
            updateEvent();

        /// ENDFRAME
        running = EndFrame();
    }
}

void Engine::SetCursorVisibility(bool i_visibility)
{
    SDL_ShowCursor(i_visibility ? SDL_ENABLE : SDL_DISABLE);
}

void Engine::SetCursorGameMode(bool i_gameMode)
{
    SDL_SetRelativeMouseMode(static_cast<SDL_bool>(i_gameMode));
}

void Engine::SetCursorPosition(const Vector2& i_position)
{
    SDL_WarpMouseInWindow(window, i_position.x, i_position.y);
}




Renderer::LowLevel::Camera* Engine::GetEditorGamera() const
{
    return &graph->editorCamera;
}

Renderer::LowLevel::Camera* Engine::GetGameCamera() const
{
    return graph->gameCamera ? &graph->gameCamera->camera : nullptr;
}




