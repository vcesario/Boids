#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"

namespace BoidManager
{
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
	extern const unsigned int BOID_AMOUNT;

	extern float AngleOfSight;
	extern float MoveSpeed;
	extern float SightRange;
	extern float CohesionFactor;
	//extern float CohesionDistance;
	extern float SeparationFactor;
	//extern float SeparationDistance;
	extern float AlignmentFactor;
	//extern float AlignmentDistance;
	extern float WallAvoidanceFactor;
}