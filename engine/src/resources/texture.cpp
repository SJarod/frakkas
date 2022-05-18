#include <stb_image.h>

#include "resources/resources_manager.hpp"
#include "debug/log.hpp"

#include "resources/texture.hpp"

void Resources::Texture::LoadFromInfo()
{
	resourceType = EResourceType::TEXTURE;

	ResourcesManager::AddCPULoadingTask([this]() {
		stbi_set_flip_vertically_on_load_thread(flip);
		data = stbi_load(name.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			ResourcesManager::CreateGPUTexture(*this);

			Log::Info("Successfully loaded texture file : " + name);

			ComputeMemorySize();
		}
		else
		{
			Log::Warning("Could not load texture file : " + name);
		}
		});
}

void Resources::Texture::ComputeMemorySize()
{
	ram = 0;
	vram = 0;

	vram += width * height * channels * sizeof(unsigned char);
}

Resources::DefaultTexture::DefaultTexture()
{
	glGenTextures(1, &data);

	glBindTexture(GL_TEXTURE_2D, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, rgb.element);

	glBindTexture(GL_TEXTURE_2D, 0);
}