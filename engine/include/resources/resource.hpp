#pragma once

#include <string>

namespace Resources
{
	enum class EResourceType
	{
		UNDEFINED,
		SHADER,
		MESH,
		SKELETALMESH,
		TEXTURE,
		ANIMPACK,
        SOUND
	};

	class Resource
	{
	public:
		virtual ~Resource() = default;

		// Resource name or path
		std::string name;
		EResourceType resourceType = EResourceType::UNDEFINED;

		// The resource's RAM size
		size_t ram = 0;

		// The resource's VRAM size
		size_t vram = 0;

		/**
		 * Load the resource from each resource specific info (name, ...).
		 * Override this function in the child class.
		 */
		virtual void LoadFromInfo() = 0;

		/**
		 * Calculate the size of this resource.
		 * Only calculate the raw resource's size (real resource data)
		 */
		virtual void ComputeMemorySize() = 0;
	};
}