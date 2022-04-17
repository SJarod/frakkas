#pragma once

#include <glad/glad.h>

#include <string>
#include <type_traits>

#include "maths.hpp"
#include "resources/resource.hpp"

namespace Resources
{
	class Shader : public Resource
	{
	public:
		/**
		 * @Summary Create a shader from a shader file.
		 * @param i_shaderName : Shader name
		 */
		Shader(const std::string &i_shaderName);
		~Shader();

		void LoadFromInfo() override;

		/**
		 * @Summary Use shader program.
		 */
		void Use() const;

        /**
        * @Summary Pass a bool to the shader as uniform.
        */
        void SetUniform(const std::string_view& i_uniformName, const bool& i_value) const;

        /**
        * @Summary Pass a Matrix4 to the shader as uniform.
        */
        void SetUniform(const std::string_view& i_uniformName, const Matrix4& i_value) const;

        /**
        * @Summary Pass a Vector3 to the shader as uniform.
        */
        void SetUniform(const std::string_view& i_uniformName, const Vector3& i_value) const;

        /**
        * @Summary Pass a Vector4 to the shader as uniform.
        */
        void SetUniform(const std::string_view& i_uniformName, const Vector4& i_value) const;

	private:
		GLuint program = 0;
	};
}
