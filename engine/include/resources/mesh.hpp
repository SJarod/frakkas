#pragma once

#include <vector>
#include <glad/glad.h>

#include "resources/resource.hpp"
#include "renderer/lowlevel/vertex.hpp"
#include "resources/texture.hpp"


using namespace Renderer::LowLevel;

namespace Resources
{
	struct GPUMesh
	{
		GLuint VBO = 0;
		GLuint VAO = 0;

		~GPUMesh()
		{
			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &VAO);
		}
	};

	class Mesh : public Resource
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		Matrix4 localTransform = Matrix4::Identity();

		GPUMesh gpu;

		// TODO : vector of Texture
		std::shared_ptr<Texture> diffuseTex;

		void LoadFromInfo() override {};
	};
}
