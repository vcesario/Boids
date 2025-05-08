#version 330 core

out vec4 FragColor;

in vec3 VPos;

void main()
{
	FragColor = vec4(VPos, 1.0);
}