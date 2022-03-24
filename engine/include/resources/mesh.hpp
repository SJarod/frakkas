#pragma once

#include <vector>

#include <glad/glad.h>

#include "renderer/lowlevel/vertex.hpp"
#include "resources/texture.hpp"

using namespace Renderer::LowLevel;

namespace Resources
{
	struct GPUMesh
	{
		GLuint VBO;
		GLuint VAO;

		/**
		 * Free OpenGL buffers.
		 * 
		 */
		~GPUMesh()
		{
			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &VAO);
		}
	};

	class Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		GPUMesh gpu;

		// TODO : vector of GPUTexture
		GPUTexture diffuseTex;
	};
}