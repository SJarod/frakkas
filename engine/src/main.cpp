//
// Created by f.marcellot on 16/02/2022.
//

#define SCRN_WIDTH 1366
#define SCRN_HEIGHT 768

#include <iostream>
#include <stdlib.h>
#include <crtdbg.h>

#include <glad/glad.h>
#include <SDL.h>

#include "maths.hpp"
#include "resources/program_shader.hpp"
#include "engine/model.hpp"
#include "renderer/lowlevel/camera.hpp"


SDL_Window* window = nullptr;
SDL_GLContext glContext;


#undef main

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		std::cout << "ERROR: Cannot init SDL - " << SDL_GetError() << std::endl;
		return false;
	}

	const char* glsl_version = "#version 330";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	window = SDL_CreateWindow("renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCRN_WIDTH, SCRN_HEIGHT, window_flags);

	if (!window)
	{
		std::cout << "ERROR: Cannot create window - " << SDL_GetError() << std::endl;
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

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	InitSDL();


	Renderer::LowLevel::Camera camera;

	Resources::Shader sh("basic");
	Engine::Model model;


	bool ShowDemoWindow = false;
	bool running = true;
	SDL_Event evt;
	int x, y;
	while (running)
	{
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_QUIT)
				running = false;
			//else if (evt.type == SDL_KEYDOWN)
			//	std::cout << "down !" << std::endl;
			//else if (evt.type == SDL_MOUSEMOTION)
			//{
			//	SDL_GetMouseState(&x, &y);
			//	std::cout << x << ", " << y << std::endl;
			//}
		}

		camera.InspectorUpdate();

		SDL_GL_SwapWindow(window);
	}

	QuitSDL();

	return 0;
}