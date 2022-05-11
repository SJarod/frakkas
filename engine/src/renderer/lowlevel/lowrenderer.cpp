#include "resources/texture.hpp"
#include "renderer/point.hpp"
#include "renderer/screen.hpp"

#include "resources/resources_manager.hpp"

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

DepthFramebuffer::DepthFramebuffer(const int i_width, const int i_height)
	: Framebuffer(i_width, i_height)
{
	glDeleteFramebuffers(1, &FBO);
	glCreateFramebuffers(1, &FBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (void*)0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attaching texture to the framebuffer's depth buffer department
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint DepthFramebuffer::GetDepthMap() const
{
	return depthMap;
}

UniformBuffer::UniformBuffer(const int i_binding, const int i_size)
	: blockBinding(i_binding), blockSize(i_size)
{
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, blockBinding, UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniformToBlock(const int i_offset, const bool i_value) const
{
	int b = static_cast<int>(i_value);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, i_offset, sizeof(int), &b);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniformToBlock(const int i_offset, const int i_value) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, i_offset, sizeof(int), &i_value);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniformToBlock(const int i_offset, const float i_value) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, i_offset, sizeof(float), &i_value);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniformToBlock(const int i_offset, const Vector3& i_value) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, i_offset, sizeof(Vector3), i_value.element);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniformToBlock(const int i_offset, const Vector4& i_value) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, i_offset, sizeof(Vector4), i_value.element);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniformToBlock(const int i_offset, const Matrix4& i_value) const
{
    Framebuffer::Unbind();
    glViewport(0, 0, 1920, 1080);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, i_offset, sizeof(Matrix4), i_value.element);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

LowRenderer::LowRenderer()
{
	shaderUBOs.insert({ "uRenderMatrices", UniformBuffer(shaderUBOs.size(), 2 * sizeof(Matrix4)) });
	shaderUBOs.insert({ "uRendering", UniformBuffer(shaderUBOs.size(), 128) });
	shaderUBOs.insert({ "uLightMatrices", UniformBuffer(shaderUBOs.size(), 2 * sizeof(Matrix4)) });
	depthMapFBO = std::make_unique<Renderer::LowLevel::DepthFramebuffer>(shadowMapResolution, shadowMapResolution);
	firstPassFBO = std::make_unique<Renderer::LowLevel::Framebuffer>(1920, 1080);
	secondPassFBO = std::make_unique<Renderer::LowLevel::Framebuffer>(1920, 1080);
}

void LowRenderer::BeginFrame(const Framebuffer& i_fbo) const
{
	i_fbo.Bind();
	glViewport(0, 0, i_fbo.GetWidth(), i_fbo.GetHeight());

	glClearColor(0.4f, 0.4f, 0.4f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void LowRenderer::ContinueFrame(const Framebuffer& i_fbo) const
{
    i_fbo.Bind();
    glViewport(0, 0, i_fbo.GetWidth(), i_fbo.GetHeight());
    glEnable(GL_DEPTH_TEST);
}

void LowRenderer::EndFrame() const
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	Framebuffer::Unbind();
}

void LowRenderer::RenderPoint(const Vector3& i_pos, const Vector3& i_color, const float i_size) const
{
	static Point point;
	point.UseShader();
	point.SetUniform("uModel", Matrix4::Translate(i_pos));
	point.SetUniform("uSize", i_size);
	point.SetUniform("uColor", i_color);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(point.VAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void LowRenderer::RenderMeshOnce(const unsigned int i_VAO, const unsigned int i_count, const unsigned int i_texture) const
{
	glBindTextureUnit(0, i_texture);
	glBindTextureUnit(1, depthMapFBO->GetDepthMap());

	glBindVertexArray(i_VAO);
	glDrawArrays(GL_TRIANGLES, 0, i_count);

	// unbind
	glBindTextureUnit(0, 0);
	glBindTextureUnit(1, 0);

	glBindVertexArray(0);
}

void LowRenderer::RenderMeshOnceOutline(const unsigned int i_VAO, const unsigned int i_count) const
{
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glLineWidth(10);

	glBindVertexArray(i_VAO);
	glDrawArrays(GL_TRIANGLES, 0, i_count);

	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// unbind
	glBindTextureUnit(0, 0);
	glBindVertexArray(0);
}

void LowRenderer::RenderLines(const unsigned int i_VAO, const unsigned int i_count, const Matrix4& i_model,
                              const float i_size, const Vector3& i_color, bool i_useLineStrip)
{
    static Point line;
    line.UseShader();
    line.SetUniform("uModel", i_model);
    line.SetUniform("uSize", i_size);
    line.SetUniform("uColor", i_color);

    glBindVertexArray(i_VAO);
    glDrawArrays(i_useLineStrip ? GL_LINE_STRIP : GL_LINES, 0, i_count);

    // unbind
    glBindVertexArray(0);
}

void LowRenderer::RenderScreen(const LowLevel::Framebuffer& i_fbo) const
{
	BeginFrame(i_fbo);

	RenderScreen();

	EndFrame();
}

void LowRenderer::RenderScreen() const
{
	static ScreenQuad sq;
	sq.UseShader();
	glBindTexture(GL_TEXTURE_2D, firstPassFBO->GetColor0());
	glBindVertexArray(sq.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}