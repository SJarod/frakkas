#pragma once

#include "game/component_generator.hpp"

namespace Renderer
{
	class Light;

	namespace LowLevel
	{
		class LowRenderer;
	}
}

namespace Game
{
	KK_PRIVATE_COMPONENT(Drawable)

		virtual void Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform = {}) {};

	KK_COMPONENT_END
}