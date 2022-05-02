#include "resources/resources_manager.hpp"

#include "renderer/render_object.hpp"

Renderer::RenderObject::RenderObject(const std::string& i_shaderName)
{
	SetCoupleShader(i_shaderName);
}

Renderer::RenderObject::RenderObject(const std::string& i_vertexShaderName, const std::string& i_fragmentShaderName)
{
	SetSingleShaders(i_vertexShaderName, i_fragmentShaderName);
}

void Renderer::RenderObject::UseShader() const
{
	shader->Use();
}

void Renderer::RenderObject::SetCoupleShader(const std::string& i_shaderName)
{
	shader = ResourcesManager::LoadResource<Resources::Shader>(i_shaderName);
}

void Renderer::RenderObject::SetSingleShaders(const std::string& i_vertexShaderName, const std::string& i_fragmentShaderName)
{
	shader = ResourcesManager::LoadResource<Resources::Shader>(i_vertexShaderName, i_fragmentShaderName);
}