#include "game/transform.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"

using namespace Renderer::LowLevel;

Framebuffer::Framebuffer(const int i_width, const int i_height)
{
	width = i_width;
	height = i_height;

	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &color0);
	glBindTexture(GL_TEXTURE_2D, color0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 i_width, i_height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glGenTextures(1, &depth);
	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 i_width, i_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (void*)0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// attaching textures to the framebuffer's corresponding department
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
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

GLuint Framebuffer::GetDepthMap() const
{
	return depth;
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
{
}

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

void LowRenderer::SetProjection(const Matrix4& i_projection) const
{
	shader.UniformMatrix4("uProjection", i_projection, false);
}

void LowRenderer::SetView(const Matrix4& i_view) const
{
	shader.UniformMatrix4("uView", i_view, false);
}

void LowRenderer::RenderMeshOnce(const Matrix4& i_model, const unsigned int i_VAO, const unsigned int i_count, const unsigned int i_texture, const bool i_hasTexture)
{
	shader.Use();
	shader.UniformMatrix4("uModel", i_model, false);

	if (i_hasTexture)
	{
		shader.UniformBool("hasTexture", true);
		glBindTexture(GL_TEXTURE_2D, i_texture);
	}

	glBindVertexArray(i_VAO);
	glDrawArrays(GL_TRIANGLES, 0, i_count);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}