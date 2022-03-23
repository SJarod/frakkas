#include <iostream>
#include <fstream>

#include "resources/program_shader.hpp"

const std::string Resources::Shader::pathToShaders = "engine/shaders";

Resources::Shader::Shader(const std::string &i_shaderName)
{
	std::string filename = pathToShaders + i_shaderName;

	//vertex shader
	std::ifstream vsStream(filename + ".vs");
	std::ifstream fsStream(filename + ".fs");

	if (!vsStream.is_open() || !fsStream.is_open())
	{
		std::cout << "could not open shader files : " << filename << std::endl;
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
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED : " << filename << ".vs\n" << infoLog[0] << std::endl;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success[1]);

	if (!success[1])
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog[1]);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED : " << filename << ".fs\n" << infoLog[1] << std::endl;
	}

	if (success[0] && success[1])
	{
		std::cout << "successfully loaded shader files : " << filename << std::endl;
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

void Resources::Shader::UniformBool(const std::string &i_uniform, const bool i_b) const
{
	glUniform1i(glGetUniformLocation(program, i_uniform.c_str()), i_b);
}

void Resources::Shader::UniformMatrix4(const std::string &i_uniform, const Matrix4& i_mat, const bool i_transpose) const
{
	glUniformMatrix4fv(glGetUniformLocation(program, i_uniform.c_str()), 1, (GLboolean)i_transpose, i_mat.element);
}