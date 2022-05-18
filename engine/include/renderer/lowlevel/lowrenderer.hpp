#pragma once

#include "maths.hpp"
#include "renderer/model.hpp"
#include "renderer/point.hpp"

#include "debug/log.hpp"

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

		protected:
			GLuint FBO = 0;

		private:
			GLuint color0 = 0;
			GLuint depthStencil = 0;

		protected:
			//framebuffer's dimensions (width)
			int width = 0;

			//framebuffer's dimensions (height)
			int height = 0;
		};

		class DepthFramebuffer : public Framebuffer
		{
		public:
			DepthFramebuffer(const int i_width, const int i_height);

			GLuint GetDepthMap() const;

		private:
			GLuint depthMap = 0;
		};

		class UniformBuffer
		{
		public:
			UniformBuffer(const int i_binding, const int i_size);

			void SetUniformToBlock(const int i_offset, const bool i_value) const;

			void SetUniformToBlock(const int i_offset, const int i_value) const;

			void SetUniformToBlock(const int i_offset, const float i_value) const;

			void SetUniformToBlock(const int i_offset, const Vector3& i_value) const;

			void SetUniformToBlock(const int i_offset, const Vector4& i_value) const;

			void SetUniformToBlock(const int i_offset, const Matrix4& i_value) const;

		private:
			GLuint UBO = 0;

			int blockBinding = -1;
			int blockSize = 0;
		};

		class LowRenderer
		{
		private:
			std::unordered_map<std::string_view, UniformBuffer> shaderUBOs;
			int shadowMapResolution = 4096;

		public:
			bool outline = true;
			//shadow rendering values
			float shadowDistance = 20.f;

			std::unique_ptr<Renderer::LowLevel::DepthFramebuffer> depthMapFBO;
			std::unique_ptr<Renderer::LowLevel::Framebuffer> firstPassFBO;
			std::unique_ptr<Renderer::LowLevel::Framebuffer> secondPassFBO;

			LowRenderer();

			/**
			 * @Summary Begin drawing on the specified the framebuffer.
			 */
			void BeginFrame(const Framebuffer& i_fbo) const;

            /**
             * @brief Drawing on a specified the framebuffer without clearing.
             * @param i_fbo The FBO to bind.
             */
            void ContinueFrame(const Framebuffer& i_fbo) const;

			/**
			 * @Summary Render is ready to be displayed.
			 */
			void EndFrame() const;

			template<typename TUniformType>
			void SetUniformToNamedBlock(const std::string_view& i_blockName, const int i_offset, const TUniformType& i_value) const;

			void RenderPoint(const Vector3& i_pos, const Vector3& i_color, const float i_size) const;

			void RenderLines(const unsigned int i_VAO, const unsigned int i_count, const Matrix4& i_model,
                             const float i_size, const Vector3& i_color, bool i_useLineStrip);

			/**
			 * @Summary Render a mesh once.
			 * Call this function in a loop to render a model in real time.
			 *
			 * @param i_VAO the opengl vertex array object
			 * @param i_count the number of triangles to draw
			 * @param i_texture the texture index to draw on the mesh
			 */
			void RenderMeshOnce(const unsigned int i_VAO, const unsigned int i_count, const unsigned int i_texture) const;

			void RenderMeshOnceOutline(const unsigned int i_VAO, const unsigned int i_count) const;

			void RenderScreen(const LowLevel::Framebuffer& i_fbo) const;

			void RenderScreen() const;
		};
	}
}

template<typename TUniformType>
void Renderer::LowLevel::LowRenderer::SetUniformToNamedBlock(const std::string_view& i_blockName, const int i_offset, const TUniformType& i_value) const
{
	if (shaderUBOs.find(i_blockName) == shaderUBOs.end())
	{
		Log::Warning(std::string(i_blockName) + " is not a uniform block");
		return;
	}

	shaderUBOs.find(i_blockName)->second.SetUniformToBlock(i_offset, i_value);
}