#pragma once

#include <glad/glad.h>

namespace Resources
{
	enum class TextureType
	{
		TEXTURE_DIFFUSE = 0
	};

	struct GPUTexture
	{
	public:
		GLuint data;
	};

	class Texture
	{
	public:
		TextureType type;
		int			width;
		int			height;
		int			bpp;

		void*		data;

		GPUTexture	gpu;
	};
}