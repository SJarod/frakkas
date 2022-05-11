#pragma once

#include "renderer/render_object.hpp"

namespace Renderer
{
	class ScreenQuad : public Renderer::RenderObject
	{
	public:
		GLuint VBO = 0;
		GLuint VAO = 0;

		/**
		 * Create a point and store it into a VAO.
		 */
		ScreenQuad();
		~ScreenQuad();
	};
}