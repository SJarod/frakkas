//
// Created by f.marcellot on 16/02/2022.
//

#include <iostream>
#include <glad/glad.h>
#include <SDL.h>
#include <backends/imgui_impl_sdl.h>

#include "maths.hpp"

#include "game/entity_manager.hpp"
#include "game/engine_entity.hpp"
#include "editor/editor_render.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"
#include "renderer/model.hpp"

SDL_Window* window = nullptr;
SDL_GLContext glContext;

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		std::cout << "ERROR: Can't init SDL - " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL4 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

	if (!window)
	{
		std::cout << "ERROR: Can't create window - " << SDL_GetError() << std::endl;
		return false;
	}

	glContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, glContext);
	SDL_GL_SetSwapInterval(1);

	// Load GL extensions using glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize the OpenGL context." << std::endl;
		exit(1);
	}

    return true;
}

void QuitSDL()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

#undef main
int main()
{
	InitSDL();

	Editor::EditorRender editorRender{};
	editorRender.InitImGui();

	ImGuiIO io = ImGui::GetIO();

	Renderer::LowLevel::LowRenderer rdr("basic");
	Renderer::LowLevel::Framebuffer fbo(1920, 1080);
	Renderer::LowLevel::Camera camera;
	camera.transform.position = { 0.f, 0.f, 2.f };

    Game::EntityManager entityManager{};
    // Create 5 entities for example
    for (int i = 0; i < 5; i++)
    {
        std::unique_ptr<Game::EngineEntity> entity = std::make_unique<Game::EngineEntity>();
        entity->GetTransform().position.x = i * 2.f;
		entity->GetTransform().scale = { 0.01f, 0.01f, 0.01f };
		entity->GetModel().AddMeshesFromFile("game/assets/bp.fbx", "game/assets/bp.jpg", false);
        entityManager.AddEntity(std::move(entity));
    }

	bool running = true;
	SDL_Event evt;
    int x = 0, y = 0;

	while (running)
	{
        float xSpeed = 0, ySpeed = 0, zSpeed = 0;
        /// INPTUS EVENT
		while (SDL_PollEvent(&evt))
		{
			ImGui_ImplSDL2_ProcessEvent(&evt);
            switch(evt.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch(evt.key.keysym.sym)
                    {
                        case SDLK_RIGHT: xSpeed = 0.064f; break;
                        case SDLK_LEFT: xSpeed = -0.064f; break;
                        case SDLK_UP: zSpeed = -0.064f; break;
                        case SDLK_DOWN: zSpeed = 0.064f; break;
                        case SDLK_SPACE: ySpeed = 0.064f; break;
                        case SDLK_LCTRL: ySpeed = -0.064f; break;
                        default: break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(evt.key.keysym.sym)
                    {
                        case SDLK_RIGHT: xSpeed = 0.f; break;
                        case SDLK_LEFT: xSpeed = 0.f; break;
                        case SDLK_UP: zSpeed = 0.f; break;
                        case SDLK_DOWN: zSpeed = 0.f; break;
                        case SDLK_SPACE: ySpeed = 0.f; break;
                        case SDLK_LCTRL: ySpeed = 0.f; break;
                        default: break;
                    }
                    break;
                case SDL_MOUSEMOTION:
				    SDL_GetMouseState(&x, &y);
                    break;
                default:
                    break;
            }
		}

        camera.transform.position.x += xSpeed;
        camera.transform.position.y += ySpeed;
        camera.transform.position.z += zSpeed;

        /// NEW FRAME

		rdr.BeginFrame(fbo);
		entityManager.Render(rdr, camera, fbo.aspectRatio);
        rdr.EndFrame();

		editorRender.UpdateAndRender(fbo, camera);

		/// ENDFRAME

		SDL_GL_SwapWindow(window);
	}

	editorRender.QuitImGui();
	QuitSDL();

	return 0;
}
