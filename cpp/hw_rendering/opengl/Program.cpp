#include <windows.h>

#include "Program.h"

Program::Program()
{
	m_programId = glCreateProgram();
}

Program::~Program()
{
	glDeleteProgram(m_programId);
}

void Program::Attach(const Shader & shader)
{
	glAttachShader(m_programId, shader.Id());
}

void Program::Detach(const Shader & shader)
{
	glDetachShader(m_programId, shader.Id());
}

void Program::Link()
{
	glLinkProgram(m_programId);

	GLint linkProgramResult = GL_FALSE;
	glGetProgramiv(m_programId, GL_LINK_STATUS, &linkProgramResult);

	if (linkProgramResult == GL_FALSE)
	{
		OutputDebugString("Failed to link program.");

		GLsizei linkProgramLogLenght = 0;
		glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &linkProgramLogLenght);
		if (linkProgramLogLenght == 0)
		{
			return;
		}

		std::vector<GLchar> linkProgramLogBuffer(linkProgramLogLenght);
		glGetProgramInfoLog(m_programId, linkProgramLogLenght, &linkProgramLogLenght, linkProgramLogBuffer.data());
		if (linkProgramLogLenght == 0)
		{
			return;
		}
	}
}

GLuint Program::Id() const
{
	return m_programId;
}
