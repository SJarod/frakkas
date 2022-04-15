#include <iostream>

#include "log.hpp"

#include "resources/program_shader.hpp"


const std::string Resources::Shader::pathToShaders = "engine/shaders/";

Resources::Shader::Shader(const std::string &i_shaderName)
{
	std::string filename = pathToShaders + i_shaderName;

	//vertex shader
	std::ifstream vsStream(filename + ".vs");
	std::ifstream fsStream(filename + ".fs");

	if (!vsStream.is_open() || !fsStream.is_open())
	{
        Log::Warning("Couldn't open shader files: \"" + filename + ".vs\" and \"" + filename + ".fs\"");
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
        Log::Error("VERTEX SHADER: \"" + filename + ".vs\" - " + infoLog[0]);
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success[1]);

	if (!success[1])
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog[1]);
        Log::Error("FRAGMENT SHADER: \"" + filename + ".fs\" - " + infoLog[1]);
	}

	if (success[0] && success[1])
	{
        Log::Info("Successfully loaded shader files: \"" + filename + ".vs\" and \"" + filename + ".fs\"");
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

Resources::Shader::~Shader()
{
    glDeleteShader(program);
}

void Resources::Shader::Use() const
{
    glUseProgram(program);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const bool& i_value) const
{
    glUniform1i(glGetUniformLocation(program, i_uniformName.data()), i_value);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const Matrix4& i_value) const
{
    glUniformMatrix4fv(glGetUniformLocation(program, i_uniformName.data()), 1, false, i_value.element);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const Vector3& i_value) const
{
    glUniform3fv(glGetUniformLocation(program, i_uniformName.data()), 1, i_value.element);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const Vector4& i_value) const
{
    glUniform4fv(glGetUniformLocation(program, i_uniformName.data()), 1, i_value.element);
}
