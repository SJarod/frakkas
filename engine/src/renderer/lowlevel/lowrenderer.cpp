#include "resources/texture.hpp"
#include "game/transform.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"


using namespace Renderer::LowLevel;

Framebuffer::Framebuffer(const int i_width, const int i_height)
{
    width = i_width;
    height = i_height;

    glCreateFramebuffers(1, &FBO);

    glCreateTextures(GL_TEXTURE_2D, 1, &color0);

    glTextureParameteri(color0, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(color0, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(color0, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(color0, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureStorage2D(color0, 1, GL_RGBA8, width, height);


    glCreateTextures(GL_TEXTURE_2D, 1, &depthStencil);

    glTextureParameteri(depthStencil, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(depthStencil, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(depthStencil, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(depthStencil, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTextureStorage2D(depthStencil, 1, GL_DEPTH24_STENCIL8, width, height);


    // attaching textures to the framebuffer's corresponding department
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, color0, 0);
    glNamedFramebufferTexture(FBO, GL_DEPTH_STENCIL_ATTACHMENT, depthStencil, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Framebuffer::GetColor0() const
{
	return color0;
}

GLuint Framebuffer::GetDepthStencilMap() const
{
	return depthStencil;
}

int Framebuffer::GetWidth() const
{
	return width;
}

int Framebuffer::GetHeight() const
{
	return height;
}

LowRenderer::LowRenderer(const std::string &i_shaderName)
	: shader(i_shaderName)
{}

void LowRenderer::BeginFrame(const Framebuffer &i_fbo) const
{
    i_fbo.Bind();
    glViewport(0, 0, i_fbo.GetWidth(), i_fbo.GetHeight());

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
}

void LowRenderer::EndFrame() const
{
	glDisable(GL_DEPTH_TEST);

	Framebuffer::Unbind();
}

void LowRenderer::RenderMeshOnce(const Matrix4& i_model, const unsigned int i_VAO, const unsigned int i_count, const unsigned int i_texture, const bool i_hasTexture)
{
    shader.SetUniform("uModel", i_model);

	if (i_hasTexture)
	{
		glBindTextureUnit(0, i_texture);
        shader.SetUniform("hasTexture", true);
    }

	glBindVertexArray(i_VAO);
    glDrawArrays(GL_TRIANGLES, 0, i_count);

    // unbind
	glBindTextureUnit(0, 0);
    glBindVertexArray(0);
}
