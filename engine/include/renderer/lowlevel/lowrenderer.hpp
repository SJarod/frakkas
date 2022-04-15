#pragma once

#include "maths.hpp"
#include "resources/program_shader.hpp"
#include "renderer/model.hpp"
#include "renderer/lowlevel/camera.hpp"
#include "game/light_component.hpp"


namespace Renderer
{
	namespace LowLevel
	{
		class Framebuffer
		{
		public:
            float aspectRatio = 16.f / 9.f;

            /**
			 * @Summary Create a framebuffer specifying its dimensions.
			 */
			Framebuffer(const int i_width, const int i_height);

			/**
			 * @Summary Use this framebuffer.
			 */
			void Bind() const;

			/**
			 * @Summary Use default framebuffer.
			 */
			static void Unbind();

			/**
			 * @Summary Get framebuffer's first color texture.
			 */
			GLuint GetColor0() const;

			/**
			 * @Summary Get framebuffer's depthStencil map.
			 */
			GLuint GetDepthStencilMap() const;

			/**
			 * @Summary Get framebuffer's width.
			 */
			int GetWidth() const;

			/**
			 * @Summary Get framebuffer's height.
			 */
			int GetHeight() const;

		private:
			GLuint FBO = 0;
			GLuint color0 = 0;
			GLuint depthStencil = 0;

			//framebuffer's dimensions (width)
			int width = 0;

			//framebuffer's dimensions (height)
			int height = 0;
		};

		class LowRenderer
		{
		public:
			/**
			 * @Summary Create a renderer given a shader name.
			 */
			LowRenderer(const std::string &i_shaderName);

			/**
			 * @Summary Begin drawing on the specified framebuffer.
			 */
            void BeginFrame(const Framebuffer& i_fbo) const;

			/**
			 * @Summary Render is ready to be displayed.
			 */
            void EndFrame() const;

            /**
            * @Summary Set the light and camera used by the shader.
            */
            //void SetUniform(const Renderer::LowLevel::Camera& i_camera, const float i_aspectRatio, const Game::LightComponent& i_light) const;

            /**
			 * @Summary Render a mesh once.
			 * Call this function in a loop to render a model in real time.
			 * 
			 * @param i_model the model matrix used by the shader to draw the mesh
			 * @param i_VAO the opengl vertex array object
			 * @param i_count the number of triangles to draw
			 * @param i_texture the texture index to draw on the mesh
			 * @param i_hasTexture specify if a texture is to be drawn on this mesh
			 */
			void RenderMeshOnce(const Matrix4& i_model, const unsigned int i_VAO, const unsigned int i_count, const unsigned int i_texture, const bool i_hasTexture);

            /**
             * @Summary send value to the shader program
             * @param i_uniformName name of the uniform, must exist in shader
             * @param i_value templated value
             */
            template <typename T>
            void SetUniform(const std::string_view& i_uniformName, const T& i_value) const;

			Resources::Shader shader;
		};
	}
}

template <typename T>
void Renderer::LowLevel::LowRenderer::SetUniform(const std::string_view &i_uniformName, const T &i_value) const
{
    shader.Use();
    shader.SetUniform(i_uniformName, i_value);
}
