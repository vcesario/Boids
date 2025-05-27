#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"

namespace BoidManager
{
	struct Light
	{
		float Yaw;
		float Pitch;
		glm::vec3 Color;
		float Intensity;
		glm::vec3 Direction;

		Light();
		Light(float yaw, float pitch, glm::vec3 color, float intensity);
		void UpdateDirection();
	};

	void Init(int screenWidth, int screenHeight);
	void Update();
	void Render(Camera camera);
	void ResetBoids();

	extern const float BOX_WIDTH;
	extern const float BOX_HALFWIDTH;
	extern const float BOX_HEIGHT;
	extern const float BOX_HALFHEIGHT;
	extern const float BOX_LENGTH;
	extern const float BOX_HALFLENGTH;
	extern const unsigned int BOID_AMOUNT;

	extern float MoveSpeed;
	extern float AngleOfSight;
	extern float SightRange;
	extern float CohesionFactor;
	extern float SeparationFactor;
	extern float AlignmentFactor;
	extern float WallAvoidanceFactor;
	extern Light DirectLight;
}