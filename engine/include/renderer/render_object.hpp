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
		RenderObject(const std::string& i_name,
			const std::string& i_shaderFilePath,
			const std::initializer_list<const std::string>& i_defines = {});

		/**
		 * Create a RenderObject using seperate shaders.
		 */
		RenderObject(const std::string& i_name,
			const std::string& i_vertexShaderFilePath,
			const std::string& i_fragmentShaderFilePath,
			const std::initializer_list<const std::string>& i_defines = {});

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

	private:
		std::shared_ptr<Resources::Shader> shader;
	};
}

template <typename... TUniform>
void Renderer::RenderObject::SetUniform(const std::string_view& i_uniformName, const TUniform&... i_params) const
{
	shader->SetUniform(i_uniformName, i_params...);
}