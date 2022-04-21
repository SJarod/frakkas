#include <iostream>

#include <glad/glad.h>
#include <SDL.h>
#include <Tracy.hpp>

#include "maths.hpp"
#include "log.hpp"
#include "game/entity.hpp"
#include "game/drawable.hpp"
#include "game/camera_component.hpp"
#include "game/light_component.hpp"
#include "game/sound_component.hpp"

#include "resources/resources_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"

#include "engine.hpp"


ma_engine Engine::soundEngine {};

Engine::Engine()
{
    Log::Init();

    InitSDL();
    InitMiniaudio();

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

void Engine::CreateTestEntities()
{
// Create 5 entities for example
    for (int i = 0; i < 5; i++)
    {
        Game::Entity* entity = entityManager.CreateEntity("entity_" + std::to_string(i));
        entity->transform.position = Vector3(i * 2.f, 0.f, 0.f);
        entity->transform.scale = Vector3(i * 0.2f + 0.2f, i * 0.2f + 0.2f, i * 0.2f + 0.2f);

        if( i >= 3)
        {
            auto drawable = entity->AddComponent<Game::Drawable>();
            auto& model = drawable->model;
            model.AddMeshesFromFile("game/assets/bp.fbx", "game/assets/bp.jpg", false);
            model.transform.scale = Vector3(0.01f, 0.01f, 0.01f);
        }
        else if (i == 1)
        {
            entity->name = "Light";
            entity->AddComponent<Game::LightComponent>();
        }
        else if (i == 2)
        {
            entity->AddComponent<Game::CameraComponent>();
            entity->name = "Game Camera";
        }
        else
        {
            Game::SoundComponent* sc = entity->AddComponent<Game::SoundComponent>();
            sc->sound.SetSound("game/assets/Airport.wav");
            entity->name = "Sound";
        }
    }

    entityManager.FindLight();
}

void Engine::Run()
{
    Game::Inputs::SetButtonAction("quit", {Game::EButton::ESCAPE});

    Game::Inputs::SetAxisAction("horizontal", {Game::EButton::ARROW_RIGHT, Game::EButton::D}, {Game::EButton::ARROW_LEFT, Game::EButton::A});

    Game::Inputs::SetAxisAction("vertical", {Game::EButton::SPACE}, {Game::EButton::LEFT_CTRL});

    Game::Inputs::SetAxisAction("forward", {Game::EButton::ARROW_UP, Game::EButton::W}, {Game::EButton::ARROW_DOWN, Game::EButton::S});

    bool running = true;
    while(running)
    {
        ResourcesManager::PollGPULoad();

        inputsManager.PollEvent();
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

        running  = Game::Inputs::IsReleased("quit") && !Game::Inputs::quit;
        SDL_GL_SwapWindow(window);

        FrameMark
    }
}