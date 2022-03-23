#include "renderer/lowlevel/lowrenderer.hpp"

void Renderer::LowLevel::Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Renderer::LowLevel::Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderer::LowLevel::LowRenderer::LowRenderer(const std::string &i_shaderName)
	: shader(i_shaderName)
{
}

void Renderer::LowLevel::LowRenderer::RenderModelOnce(const Engine::Model& i_model, const Matrix4& i_view, const Matrix4& i_projection)
{
	if (!i_model.IsDrawable())
		return;

    glEnable(GL_DEPTH_TEST);

	shader.Use();
	shader.UniformMatrix4("uProjection", i_projection, false);
	shader.UniformMatrix4("uView", i_view, false);

	shader.UniformMatrix4("uModel", i_model.transform.GetModelMatrix(), false);
	shader.UniformBool("hasTexture", false);

	for (const auto & mesh : i_model.meshes)
	{
		glBindTexture(GL_TEXTURE_2D, mesh->diffuseTex.data);

		glBindVertexArray(mesh->gpu.VAO);
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

    glDisable(GL_DEPTH_TEST);
}

void LowRenderer::BeginDraw(const Framebuffer &i_fbo) const {
    i_fbo.Bind();
    glViewport(0, 0, 1000, 1000);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LowRenderer::EndDraw() const {
    Framebuffer::Unbind();
}

Renderer::LowLevel::Framebuffer::Framebuffer(const int i_width, const int i_height)
{
	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &color);
	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 i_width, i_height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &depth);
	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 i_width, i_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (void*)0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glBindTexture(GL_TEXTURE_2D, 0);

	// attaching textures to the framebuffer's corresponding department
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


