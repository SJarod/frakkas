#pragma once

#include <glad/glad.h>

#include "maths.hpp"

#include "resources/resource.hpp"

namespace Resources
{
	enum class ETextureType
	{
		TEXTURE_DIFFUSE = 0
	};

	struct GPUTexture
	{
	public:
		GLuint data;
	};

	class Texture : public Resource
	{
	public:
		ETextureType	type = ETextureType::TEXTURE_DIFFUSE;
		bool			flip = false;

		int				width = 0;
		int				height = 0;
		int				bpp = 0;

		void*			data = nullptr;

		std::unique_ptr<GPUTexture> gpu;

		Texture(const std::string& i_name, const bool i_flip)
		{
			name = i_name;
			flip = i_flip;
		}

		void LoadFromInfo() override;
	};

	class DefaultTexture
	{
	public:
		Vector3 rgb = { 1.f, 0.f, 1.f };
		GLuint data;

		DefaultTexture();
	};
}
