#pragma once

#include "maths.hpp"

#include "resources/program_shader.hpp"
#include "renderer/model.hpp"

namespace Renderer
{
	namespace LowLevel
	{
        class Framebuffer;

		class LowRenderer
		{
		public:
			LowRenderer(const std::string &i_shaderName);

			/**
			 * Render a model once.
			 * Call this function in a loop to render in real time.
			 * 
			 * @param i_model object
			 * @param i_view matrix
			 * @param i_projection matrix
			 */
			void RenderModelOnce(const Model& i_model, const Matrix4& i_view, const Matrix4& i_projection);

            void BeginDraw(const Framebuffer& i_fbo) const;
            void EndDraw() const;

		private:
			Resources::Shader shader;
		};

		class Framebuffer
		{
		public:
			GLuint FBO = 0;
			GLuint color = 0;
			GLuint depth = 0;

			Framebuffer(const int i_width, const int i_height);

			void Bind() const;
			static void Unbind();
		};
	}
}
