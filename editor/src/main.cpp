
#include <iostream>
#include <glad/glad.h>
#include <SDL.h>
#include <backends/imgui_impl_sdl.h>

#include "engine/entity_manager.hpp"
#include "editor/editor_render.hpp"
#include "maths.hpp"

#define PROCESS_EDITOR


SDL_Window* window = nullptr;
SDL_GLContext glContext;

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		std::cout << "ERROR: Can't init SDL - " << SDL_GetError() << std::endl;
		return false;
	}

	const char* glsl_version = "#version 450";
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
    Editor::EditorRender editorRender;
    editorRender.InitImGui();


	bool ShowDemoWindow = true;
	bool running = true;
	SDL_Event evt;

	while (running)
	{
		while (SDL_PollEvent(&evt))
		{
			ImGui_ImplSDL2_ProcessEvent(&evt);

			if (evt.type == SDL_QUIT)
				running = false;
		}

		/// NEW FRAME

#ifdef PROCESS_EDITOR
        editorRender.UpdateAndRender(ShowDemoWindow);
#else
		if (gaming)
            Engine::EntityManager::UpdateAndRender();
#endif

		/// ENDFRAME

		SDL_GL_SwapWindow(window);
	}

    editorRender.QuitImGui();
    QuitSDL();

	return 0;
}
