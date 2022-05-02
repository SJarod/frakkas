#include <iostream>

#include "debug/log.hpp"
#include "resources/resources_manager.hpp"

#include "resources/program_shader.hpp"

Resources::Shader::Shader(const std::string& i_shaderName)
	: vertexShaderName(i_shaderName), fragmentShaderName(i_shaderName)
{
	name = i_shaderName;
}

Resources::Shader::Shader(const std::string& i_vertexShaderName, const std::string& i_fragmentShaderName)
	: vertexShaderName(i_vertexShaderName), fragmentShaderName(i_fragmentShaderName)
{
	name = i_vertexShaderName + "+" + i_fragmentShaderName;
}

Resources::Shader::~Shader()
{
	glDeleteShader(program);
}

void Resources::Shader::LoadFromInfo()
{
	//vertex shader
	std::ifstream vsStream(vertexShaderName + ".vs");
	std::ifstream fsStream(fragmentShaderName + ".fs");

	if (!vsStream.is_open() || !fsStream.is_open())
	{
		Log::Error("Could not open shader files : " + name);
		return;
	}

	std::string vs((std::istreambuf_iterator<char>(vsStream)), std::istreambuf_iterator<char>());
	std::string fs((std::istreambuf_iterator<char>(fsStream)), std::istreambuf_iterator<char>());

	const char* vertexShaderSource = vs.c_str();
	const char* fragmentShaderSource = fs.c_str();

	//shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	int  success[2];
	char infoLog[2][512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success[0]);

	if (!success[0])
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog[0]);
		Log::Error("ERROR::SHADER::VERTEX::COMPILATION_FAILED : " + vertexShaderName + ".vs\n" + infoLog[0]);
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success[1]);

	if (!success[1])
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog[1]);
		Log::Error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED : " + fragmentShaderName + ".fs\n" + infoLog[1]);
	}

	if (success[0] && success[1])
	{
		Log::Info("Successfully loaded shader files : " + name);

		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	vsStream.close();
	fsStream.close();
}

void Resources::Shader::Use() const
{
	glUseProgram(program);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const bool& i_value) const
{
	int location = glGetUniformLocation(program, i_uniformName.data());
	if (location < 0)
	{
		Log::Info(std::string(i_uniformName) + " is not a shader uniform in " + name);
		return;
	}
	glUniform1i(location, i_value);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const float& i_value) const
{
	int location = glGetUniformLocation(program, i_uniformName.data());
	if (location < 0)
	{
		Log::Info(std::string(i_uniformName) + " is not a shader uniform in " + name);
		return;
	}
	glUniform1f(location, i_value);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const Matrix4& i_value, const bool i_transpose) const
{
	int location = glGetUniformLocation(program, i_uniformName.data());
	if (location < 0)
	{
		Log::Info(std::string(i_uniformName) + " is not a shader uniform in " + name);
		return;
	}
	glUniformMatrix4fv(location, 1, i_transpose, i_value.element);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const int i_num, const float* i_array, const bool i_transpose) const
{
	int location = glGetUniformLocation(program, i_uniformName.data());
	if (location < 0)
	{
		Log::Info(std::string(i_uniformName) + " is not a shader uniform in " + name);
		return;
	}
	glUniformMatrix4fv(location, i_num, i_transpose, i_array);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const Vector3& i_value) const
{
	int location = glGetUniformLocation(program, i_uniformName.data());
	if (location < 0)
	{
		Log::Info(std::string(i_uniformName) + " is not a shader uniform in " + name);
		return;
	}
	glUniform3fv(location, 1, i_value.element);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const Vector4& i_value) const
{
	int location = glGetUniformLocation(program, i_uniformName.data());
	if (location < 0)
	{
		Log::Info(std::string(i_uniformName) + " is not a shader uniform in " + name);
		return;
	}
	glUniform4fv(location, 1, i_value.element);
}
