#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"

namespace BoidManager
{
	enum Axis
	{
		XAXIS,
		YAXIS,
		ZAXIS
	};

	struct Boid
	{
		glm::vec3 Velocity;
		glm::vec3 Position;

		Boid(glm::vec3 position, glm::vec3 velocity)
		{
			Position = position;
			Velocity = velocity;
		}
	};

	void Init(int screenWidth, int screenHeight);
	void Update(float deltaTime);
	void Render(Camera camera);
	void ResetBoids();

	extern const float BOX_WIDTH;
	extern const float BOX_HALFWIDTH;
	extern const float BOX_HEIGHT;
	extern const float BOX_HALFHEIGHT;
	extern const float BOX_LENGTH;
	extern const float BOX_HALFLENGTH;
	extern const GLubyte BOID_AMOUNT;

	extern float MoveSpeed;
	extern float CohesionFactor;
	extern float CohesionDistance;
	extern float SeparationFactor;
	extern float SeparationDistance;
	extern float AlignmentFactor;
	extern float AlignmentDistance;
}