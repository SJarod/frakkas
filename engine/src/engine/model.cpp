#include <glad/glad.h>

#include "engine/model.hpp"

void ParseMesh(Resources::Mesh& io_mesh)
{
	std::vector<Vertex> rawVertices = io_mesh.vertices;
	io_mesh.vertices.clear();

	// process indices
	int meshNumFaces = io_mesh.indices.size();
	for (unsigned int i = 0; i < meshNumFaces; ++i)
	{
		io_mesh.vertices.push_back(rawVertices[io_mesh.indices[i]]);
	}
}

void CreateGPUMesh(Resources::Mesh& io_mesh)
{
	ParseMesh(io_mesh);

	glGenBuffers(1, &io_mesh.gpu.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, io_mesh.gpu.VBO);
	glBufferData(GL_ARRAY_BUFFER, io_mesh.vertices.size() * sizeof(Vertex), io_mesh.vertices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &io_mesh.gpu.VAO);
	glBindVertexArray(io_mesh.gpu.VAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}

Engine::Model::Model()
{
	Resources::Mesh mesh;
	mesh.vertices = {
		{ { -1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f } },
		{ { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f }, { 0.f, 1.f } },
		{ { 0.f, 1.f, 0.f }, { 0.f, 0.f, 1.f }, { 1.f, 1.f } }
	};

	mesh.indices = { 0, 1, 2 };

	meshes.push_back(std::make_shared<Resources::Mesh>(mesh));
	CreateGPUMesh(*meshes.back());
}

bool Engine::Model::IsDrawable() const
{
	return !meshes.empty();
}