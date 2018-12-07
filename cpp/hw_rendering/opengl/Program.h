#pragma once

#include <GL/glew.h>

#include "Shader.h"

class Program
{
public:
	Program();
	~Program();

	void Attach(const Shader & shader);
	void Detach(const Shader & shader);

	void Link();

	GLuint Id() const;

private:
	GLuint m_programId;
};
