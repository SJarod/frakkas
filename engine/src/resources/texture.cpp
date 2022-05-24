#include <stb_image.h>

#include "resources/resources_manager.hpp"
#include "debug/log.hpp"

#include "resources/texture.hpp"

bool Resources::Texture::CPULoad()
{
	resourceType = EResourceType::TEXTURE;

	stbi_set_flip_vertically_on_load_thread(flip);
	data = stbi_load(name.c_str(), &width, &height, &channels, 0);

	if (data)
	{
		Log::Info("Successfully loaded texture file : " + name);

		ram = 0;
		vram = 0;

		vram += width * height * channels * sizeof(unsigned char);

		return true;
	}
	else
	{
		Log::Warning("Could not load texture file : " + name);
		return false;
	}
}

bool Resources::Texture::GPULoad()
{
	gpu = std::make_unique<GPUTexture>();
	glGenTextures(1, &gpu->data);

	glBindTexture(GL_TEXTURE_2D, gpu->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data)
	{
		GLint format = channels == 1 ? GL_RED : channels == 2 ? GL_RG : channels == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return true;
	}
	return false;
}

bool Resources::Texture::CPUUnload()
{
	if (data)
	{
		stbi_image_free(data);
		return true;
	}
	return false;
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