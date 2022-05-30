#include <iostream>

#include <glad/glad.h>
#include <SDL.h>
#include <Tracy.hpp>

#include "maths.hpp"
#include "debug/log.hpp"
#include "game/entity.hpp"
#include "game/lowcomponent/camera.hpp"

#include "resources/resources_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/graph.hpp"

#include "engine.hpp"

#ifdef KK_WINDOWS
// Run on laptop high perf GPU
extern "C"
{
    __declspec(dllexport) int NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

SDL_Window* Engine::window = nullptr;
ma_engine Engine::soundEngine {};

inline Vector2 AdaptSize(Vector2& size, float aspectRatio)
{
    Vector2 oldSize = size, offset;
    float newAspect = size.x / size.y;
    if (newAspect > aspectRatio)
        size.x = aspectRatio * size.y;
    else if (newAspect < aspectRatio)
        size.y = size.x / aspectRatio;

    offset.x = (oldSize.x - size.x) / 2.f;
    offset.y = (oldSize.y - size.y) / 2.f;

    return offset;
}

Engine::Engine()
{
    Log::Init();

    InitSDL();
    InitMiniaudio();

    renderer = std::make_unique<Renderer::LowLevel::LowRenderer>();
    editorFBO = std::make_unique<Renderer::LowLevel::Framebuffer>(1920, 1080, Renderer::LowLevel::Framebuffer::ERenderMode::FREE_ASPECT);
    gameFBO = std::make_unique<Renderer::LowLevel::Framebuffer>(1920, 1080, Renderer::LowLevel::Framebuffer::ERenderMode::LOCK_ASPECT);

    graph = std::make_unique<Renderer::Graph>(&entityManager, &physicScene, renderer.get());
}

Engine::~Engine()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    ma_engine_uninit(&soundEngine);
}

static void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        return;
    // Ignore issues that we can't fix
    if (std::string(message).find("being recompiled based on GL state"))
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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
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
}

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

    bool quit = Game::Inputs::quit;
    if (quit)
        ResourcesManager::DestroyResources();
    return  !quit;
}

void Engine::RunEditor()
{
    bool running = true;
    while(running)
    {
        BeginFrame();
        
        /// OnUpdate
        if (updateMode & Utils::UpdateFlag_Gaming)
        {
            // Disable inputs for game update if editing
            if (updateMode & Utils::UpdateFlag_Editing)
                DisableInputs();

            entityManager.Update();
        }

        EnableInputs();
        for (const UpdateEvent& updateEvent : updateEventsHandler)
            updateEvent();

        physicScene.Update(updateMode);

        /// Draw
        graph->RenderEditor(*renderer, editorFBO->AspectRatio());
        renderer->RenderFinalScreen(*editorFBO);

        graph->RenderGame(*renderer, gameFBO->AspectRatio());
        renderer->RenderFinalScreen(*gameFBO);

		running = EndFrame();
    }
}

void Engine::RunGame()
{
    EnableInputs();
    SetRunMode(Utils::UpdateFlag_Gaming);

    int width = 0, height = 0;
    Vector2 windowSize(width, height);

    gameFBO->lockAspectRatio = true;

    bool running = true;
    while(running)
    {
        BeginFrame();

		entityManager.Update();

        for (const UpdateEvent& updateEvent : updateEventsHandler)
            updateEvent();

        physicScene.Update();

        SDL_GetWindowSize(window, &width, &height);
        windowSize = {static_cast<float>(width), static_cast<float>(height)};

        renderer->SetWindowSize(windowSize);

        graph->RenderGame(*renderer, renderer->firstPassFBO->AspectRatio());
        glViewport(renderer->firstPassFBO->offset.x, renderer->firstPassFBO->offset.y, windowSize.x, windowSize.y);
        renderer->RenderFinalScreen();
        
        /// ENDFRAME
        running = EndFrame();
    }
}

void Engine::SetRunMode(unsigned int i_flag)
{
    updateMode = i_flag;
    graph->playing = updateMode & Utils::UpdateFlag_Gaming;

    if (updateMode & Utils::UpdateFlag_Gaming)
        Engine::SetCursorGameMode(true);

    if (updateMode & Utils::UpdateFlag_Editing)
        Engine::SetCursorGameMode(false);

    if (updateMode & Utils::UpdateFlag_Gaming && gameRestart)
    {
        gameRestart = false;
        entityManager.Start();
    }

}

Utils::UpdateFlag Engine::GetRunMode() const
{
    return updateMode;
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

void Engine::EnableInputs()
{
    inputsManager.SetInputsListening(true);
}

void Engine::DisableInputs()
{
    inputsManager.SetInputsListening(false);
}

Game::Camera* Engine::GetEditorGamera() const
{
    return graph->editorCamera;
}

Game::Camera* Engine::GetGameCamera() const
{
    return graph->gameCamera;
}

Game::Transform& Engine::GetEditorCameraTransform() const
{
    return graph->editorCameraman.transform;
}



