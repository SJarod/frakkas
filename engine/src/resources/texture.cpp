#include <stb_image.h>

#include "resources/resources_manager.hpp"
#include "debug/log.hpp"

#include "resources/texture.hpp"

void Resources::Texture::LoadFromInfo()
{
	ResourcesManager::AddCPULoadingTask([this]() {
		stbi_set_flip_vertically_on_load(flip);
		data = stbi_load(name.c_str(), &width, &height, &bpp, 0);

		if (data)
		{
			ResourcesManager::CreateGPUTexture(*this);

			Log::Info("Successfully loaded texture file : " + name);
		}
		else
		{
			Log::Warning("Could not load texture file : " + name);
		}
		});
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