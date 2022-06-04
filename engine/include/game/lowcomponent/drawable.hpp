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

		virtual void Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform) {}

		virtual void DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform) {}

        /**
         * @brief Set a new mesh resource to this drawable object.
         */
        virtual void SetMesh(const std::string& i_path) {};
        /**
         * @brief Set a new texture resource to this drawable object.
         */
        virtual void SetTexture(const std::string& i_path, bool i_flip) {};

	KK_COMPONENT_END
}