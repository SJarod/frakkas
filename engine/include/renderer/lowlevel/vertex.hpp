#pragma once

#include "maths.hpp"

namespace Renderer
{
	namespace LowLevel
	{
		struct Vertex
		{
		public:
			Vector3 position;
			Vector3 normal;
			Vector2 uv;
		};
	};
};