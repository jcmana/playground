#include <windows.h>

#include "Shader.h"

Shader::Shader(GLenum shaderType, const std::string & shaderCode) :
	m_shaderId(0)
{
	m_shaderId = glCreateShader(shaderType);

	const GLchar * shaderCodeTemp = shaderCode.c_str();
	glShaderSource(m_shaderId, 1, &shaderCodeTemp, NULL);

	glCompileShader(m_shaderId);

	GLint shaderCompileStatus = GL_FALSE;
	glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &shaderCompileStatus);
	if (shaderCompileStatus == GL_FALSE)
	{
		OutputDebugString("Failed to compile shader");

		GLint sharedLogLenght = 0;
		glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &sharedLogLenght);
		if (sharedLogLenght == 0)
		{
			OutputDebugString("Empty shader info log.");
			return;
		}

		std::vector<GLchar> sharedLogBuffer(sharedLogLenght);
		glGetShaderInfoLog(m_shaderId, sharedLogLenght, &sharedLogLenght, sharedLogBuffer.data());
		if (sharedLogLenght == 0)
		{
			OutputDebugString("Empty shader info log.");
			return;
		}
	}
}

Shader::~Shader()
{
	glDeleteShader(m_shaderId);
}

GLuint Shader::Id() const
{
	return m_shaderId;
}
