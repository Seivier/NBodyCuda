//
// Created by Vicente González on 18-06-23.
//

#include <fstream>
#include <string>
#include <iostream>

#include "shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	: _id(0)
{
	_id = glCreateProgram();
	auto vertex = loadSources(GL_VERTEX_SHADER, vertexPath);
	auto fragment = loadSources(GL_FRAGMENT_SHADER, fragmentPath);

	linkSources(vertex, fragment);
}

Shader::Shader(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath)
	: _id(0)
{
	_id = glCreateProgram();
	auto vertex = loadSources(GL_VERTEX_SHADER, vertexPath);
	auto geometry = loadSources(GL_GEOMETRY_SHADER, geometryPath);
	auto fragment = loadSources(GL_FRAGMENT_SHADER, fragmentPath);

	linkSources(vertex, geometry, fragment);
}

Shader::~Shader()
{
	glDeleteProgram(_id);
}

unsigned int Shader::loadSources(unsigned int type, const std::string& path)
{
	auto shader = glCreateShader(type);
	std::ifstream shaderFile(path);
	std::string shaderSource((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	const char* shaderSourcePtr = shaderSource.c_str();
	glShaderSource(shader, 1, &shaderSourcePtr, nullptr);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Shader compilation error: " << infoLog << std::endl;
	}

	return shader;
}

void Shader::linkSources(unsigned int vertex, unsigned int fragment) const
{
	glAttachShader(_id, vertex);
	glAttachShader(_id, fragment);
	glLinkProgram(_id);

	int success;
	char infoLog[512];
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_id, 512, nullptr, infoLog);
		std::cerr << "Shader linking error: " << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::linkSources(unsigned int vertex, unsigned int geometry, unsigned int fragment) const
{
	glAttachShader(_id, vertex);
	glAttachShader(_id, geometry);
	glAttachShader(_id, fragment);
	glLinkProgram(_id);

	int success;
	char infoLog[512];
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_id, 512, nullptr, infoLog);
		std::cerr << "Shader linking error: " << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(geometry);
	glDeleteShader(fragment);
}
void Shader::bind() const
{
	glUseProgram(_id);
}
void Shader::unbind() const
{
	glUseProgram(0);
}
