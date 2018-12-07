#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <GL/glew.h>

static constexpr unsigned int SHADER_LOG_BUFFER_SIZE = 1024;

class Shader
{
public:
	Shader(GLenum shaderType, const std::string & shaderCode);
	~Shader();

	GLuint Id() const;

private:
	unsigned int m_shaderId;
};