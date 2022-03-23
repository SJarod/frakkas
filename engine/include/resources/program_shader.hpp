#pragma once

#include <glad/glad.h>

#include <string>

#include "maths.hpp"

namespace Resources
{
	class Shader
	{
	public:

        static const std::string pathToShaders;
		/**
		 * Create a shader from a shader file.
		 * 
		 * @param i_shaderName
		 */
		Shader(const std::string &i_shaderName);
		~Shader();

		/**
		 * Use shader program.
		 */
		void Use() const;

		/**
		 * Pass a bool to the shader as uniform.
		 * 
		 * @param i_uniform : uniform name
		 * @param i_b
		 */
		void UniformBool(const std::string &i_uniform, const bool i_b) const;

		/**
		 * Pass a Matrix4 to the shader as uniform.
		 * 
		 * @param i_uniform : uniform name
		 * @param i_mat
		 * @param i_transpose
		 */
		void UniformMatrix4(const std::string &i_uniform, const Matrix4& i_mat, const bool i_transpose) const;

	private:
		GLuint program = 0;
	};
}