#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "debug/log.hpp"
#include "maths.hpp"

#include "resources/resources_manager.hpp"

Resources::ResourcesManager::~ResourcesManager()
{
	for (auto& resource : resources)
	{
		if (std::shared_ptr<Texture> texture = std::dynamic_pointer_cast<Texture>(resource.second))
			if (texture->data)
				stbi_image_free(texture->data);
	}
}

void Resources::ResourcesManager::CreateGPUSubmesh(Submesh& io_mesh)
{
	ResourcesManager& rm = ResourcesManager::Instance();

	std::lock_guard<std::mutex> guard(rm.loadMX);
	rm.gpuLoadQueue.push_back([&]() {
		glGenBuffers(1, &io_mesh.gpu.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, io_mesh.gpu.VBO);
		glBufferData(GL_ARRAY_BUFFER, io_mesh.vertices.size() * sizeof(Vertex), io_mesh.vertices.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &io_mesh.gpu.VAO);
		glBindVertexArray(io_mesh.gpu.VAO);

#if 1
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIndices));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneWeights));
#else
		glVertexAttrib3f(0, 0.f, 0.f, 0.f);
		glVertexAttrib3f(1, 0.f, 0.f, 0.f);
		glVertexAttrib2f(2, 0.f, 0.f);
#endif

		glBindVertexArray(0);
		});
}

void Resources::ResourcesManager::CreateGPUTexture(Texture& io_texture)
{
	ResourcesManager& rm = ResourcesManager::Instance();

	std::lock_guard<std::mutex> guard(rm.loadMX);
	rm.gpuLoadQueue.push_back([&]() {
		io_texture.gpu = std::make_unique<GPUTexture>();
		glGenTextures(1, &io_texture.gpu->data);

		glBindTexture(GL_TEXTURE_2D, io_texture.gpu->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (io_texture.data)
		{
			GLint bpp = io_texture.bpp == 1 ? GL_RED : io_texture.bpp == 2 ? GL_RG : io_texture.bpp == 3 ? GL_RGB : GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, bpp, io_texture.width, io_texture.height, 0, bpp, GL_UNSIGNED_BYTE, io_texture.data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		});
}

const DefaultTexture& Resources::ResourcesManager::GetDefaultTexture()
{
	return ResourcesManager::Instance().defaultTexture;
}

void Resources::ResourcesManager::AddCPULoadingTask(Task io_task)
{
	ResourcesManager::Instance().tp.AddTask(io_task);
}

void Resources::ResourcesManager::PollGPULoad()
{
	ResourcesManager& rm = ResourcesManager::Instance();

	std::lock_guard<std::mutex> guard(rm.loadMX);

	for (Task& task : rm.gpuLoadQueue)
	{
		task();
	}

	rm.gpuLoadQueue.clear();
}