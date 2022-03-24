//
// Created by f.marcellot on 16/02/2022.
//

#include <iostream>
#include <glad/glad.h>
#include <SDL.h>
#include <backends/imgui_impl_sdl.h>

#include "engine/entity_manager.hpp"
#include "engine/engine_entity.hpp"
#include "editor/editor_render.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/lowlevel/camera.hpp"

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
	Renderer::LowLevel::Framebuffer fbo(1000, 1000);
	Renderer::LowLevel::Camera camera;
	camera.transform.position = { 0.f, 0.f, 2.f };

    Engine::EntityManager entityManager{};
    // Create 5 entities for example
    for (int i = 0; i < 5; i++)
    {
        std::unique_ptr<Engine::EngineEntity> entity = std::make_unique<Engine::EngineEntity>();
        entity->GetTransform().position.x = i * 2.f;
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

		editorRender.UpdateAndRender(fbo);

        /// DRAW

		rdr.BeginDraw(fbo);

		entityManager.Render(rdr, camera);

        rdr.EndDraw();

		/// ENDFRAME

		SDL_GL_SwapWindow(window);
	}

	editorRender.QuitImGui();
	QuitSDL();

	return 0;
}
