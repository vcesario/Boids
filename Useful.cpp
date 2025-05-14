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