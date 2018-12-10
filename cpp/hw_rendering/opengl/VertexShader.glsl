#version 330 core
layout(location = 0) in vec2 vertex;

uniform mat3 transform;

void main()
{
	gl_Position = vec4(vertex, 0.0, 1.0);
}
