#include <glad/glad.h>

#include "Useful.h"

std::string ToString(glm::vec3 vector)
{
	char buffer[64];
	std::snprintf(buffer, sizeof(buffer), "(%.2f, %.2f, %.2f)", vector.x, vector.y, vector.z);
	return std::string(buffer);
}

std::string YawPitchToString(float yaw, float pitch)
{
	char buffer[64];
	std::snprintf(buffer, sizeof(buffer), "(%.2f, %.2f)", yaw, pitch);
	return std::string(buffer);
}

void SetArrayBuffer(unsigned int* vao, const void* vData, GLsizeiptr vSize, const void* iData, GLsizeiptr iSize)
{
	unsigned int vbo;
	unsigned int ebo;

	glGenVertexArrays(1, vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(*vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vSize, vData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, iData, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}