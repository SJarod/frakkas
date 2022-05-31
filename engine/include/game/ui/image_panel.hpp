#pragma once

#include <memory>

#include "game/ui/ui_object.hpp"
#include "game/component_generator.hpp"

namespace Resources
{
    class Texture;
}

namespace Game
{
    KK_COMPONENT_FROM(ImagePanel, UIObject)

        std::string texturePath;
        bool imageFlip = false;

        void SetTexture(const std::string& i_texturePath);

        void Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const override;

    private:
        std::shared_ptr<Resources::Texture> texture;

    KK_COMPONENT_END
}

void OnImagePanelTextureSet(unsigned char* io_component);
void OnImagePanelTextureDrop(unsigned char* io_component, void* io_data);