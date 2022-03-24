#pragma once

#include <memory>
#include <vector>

#include "resources/mesh.hpp"
#include "resources/texture.hpp"
#include "resources/program_shader.hpp"
#include "../../../physics/include/transform.hpp"

namespace Renderer
{
	class Model
	{
	public:
		// Every meshes that the model posseses.
		std::vector<std::shared_ptr<Resources::Mesh>> meshes;

		Physics::Transform transform;

		/**
		 * Create a simple triangle.
		 * 
		 */
		Model();

		/**
		 * Return true if this model can be drawn, if the array of meshes is not empty.
		 * 
		 * @return 
		 */
		[[nodiscard]] bool IsDrawable() const;
	};
};
