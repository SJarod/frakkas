#pragma once

#include "renderer/skeletal_model.hpp"
#include "game/lowcomponent/drawable.hpp"

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT_FROM(AnimatedDraw, Drawable)

        Renderer::SkeletalModel skmodel;

        std::string meshPath = "none";
        std::string texturePath = "none";
        std::string animationPath = "none";

        void SetMesh(const std::string& i_path) override;
        void SetTexture(const std::string& i_path, bool i_flip) override;
        void SetAnimation(const std::string& i_path);

protected:
    void OnUpdate() override;

    void Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform = {}) override;

    void DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform) override;

    KK_COMPONENT_END
}

void DropOnAnimatedDrawComponent(unsigned char* io_component, void* io_dropData);
void AnimationDropOnAnimatedDrawComponent(unsigned char* io_component, void* io_dropData);
void OnAnimationMeshPathUpdate(unsigned char* io_component);
void OnAnimationTexturePathUpdate(unsigned char* io_component);
void OnAnimationPathUpdate(unsigned char* io_component);