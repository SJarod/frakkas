#pragma once

#include "game/lowcomponent/component.hpp"

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
	class Drawable : public Component
	{
	public:
		virtual void Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light) = 0;
	};
}