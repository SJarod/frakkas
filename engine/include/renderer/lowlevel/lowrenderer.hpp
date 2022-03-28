#pragma once

#include "maths.hpp"

#include "resources/program_shader.hpp"
#include "renderer/model.hpp"

namespace Renderer
{
	namespace LowLevel
	{
		class Framebuffer
		{
		public:
			/**
			 * Create a framebuffer specifying its dimensions.
			 */
			Framebuffer(const int i_width, const int i_height);

			/**
			 * Use this framebuffer.
			 */
			void Bind() const;

			/**
			 * Use default framebuffer.
			 */
			static void Unbind();

			/**
			 * Get framebuffer's first color texture.
			 */
			GLuint GetColor0() const;

			/**
			 * Get framebuffer's depth map.
			 */
			GLuint GetDepthMap() const;

			/**
			 * Get framebuffer's width.
			 */
			int GetWidth() const;

			/**
			 * Get framebuffer's height.
			 */
			int GetHeight() const;

		private:
			GLuint FBO = 0;
			GLuint color0 = 0;
			GLuint depth = 0;

			//framebuffer's dimensions (width)
			int width = 0;

			//framebuffer's dimensions (height)
			int height = 0;

		public:
			float aspectRatio = 16.f / 9.f;
		};

		class LowRenderer
		{
		public:
			/**
			 * Create a renderer given a shader name.
			 */
			LowRenderer(const std::string &i_shaderName);

			/**
			 * Begin drawing on the specified framebuffer.
			 */
            void BeginFrame(const Framebuffer& i_fbo) const;

			/**
			 * Render is ready to be displayed.
			 */
            void EndFrame() const;

			/**
			 * Set the projection matrix used by the shader to draw the models.
			 */
			void SetProjection(const Matrix4& i_projection) const;

			/**
			 * Set the view matrix used by the shader to draw the models.
			 */
			void SetView(const Matrix4& i_view) const;

			/**
			 * Render a mesh once.
			 * Call this function in a loop to render a model in real time.
			 * 
			 * @param i_model the model matrix used by the shader to draw the mesh
			 * @param i_VAO the opengl vertex array object
			 * @param i_count the number of triangles to draw
			 * @param i_texture the texture index to draw on the mesh
			 * @param i_hasTexture specify if a texture is to be drawn on this mesh
			 */
			void RenderMeshOnce(const Matrix4& i_model,
								const unsigned int i_VAO,
								const unsigned int i_count,
								const unsigned int i_texture,
								const bool i_hasTexture);

		private:
			Resources::Shader shader;
		};
	}
}