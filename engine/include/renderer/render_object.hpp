#pragma once

#include "resources/program_shader.hpp"

namespace Renderer
{
	/**
	 * RenderObjects are renderable.
	 */
	class RenderObject
	{
	public:
		/**
		 * Create a RenderObject using a set of shaders.
		 */
		RenderObject(const std::string& i_shaderName);

		/**
		 * Create a RenderObject using seperate shaders.
		 */
		RenderObject(const std::string& i_vertexShaderName, const std::string& i_fragmentShaderName);

		/**
		 * @Summary send value to the shader program
		 * @param i_uniformName name of the uniform, must exist in shader
		 * @param i_value templated value
		 */
		template <typename... TUniform>
		void SetUniform(const std::string_view& i_uniformName, const TUniform&... i_params) const;

		/**
		 * Use this RenderObject's shader.
		 */
		void UseShader() const;

		/**
		 * Give this RenderObject a shader program based on a set of shaders.
		 */
		void SetCoupleShader(const std::string& i_shaderName);

		/**
		 * Give this RenderObject a shader program based on two seperate shaders.
		 */
		void SetSingleShaders(const std::string& i_vertexShaderName, const std::string& i_fragmentShaderName);

	private:
		std::shared_ptr<Resources::Shader> shader;
	};
}

template <typename... TUniform>
void Renderer::RenderObject::SetUniform(const std::string_view& i_uniformName, const TUniform&... i_params) const
{
	shader->SetUniform(i_uniformName, i_params...);
}