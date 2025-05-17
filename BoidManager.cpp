#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "BoidManager.h"
#include "Useful.h"

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
		glm::vec3 Position;
		glm::vec3 Velocity;
		glm::vec3 Front;
		glm::vec3 Right;
		glm::vec3 Up;
		glm::vec3 WorldUp;

		Boid(glm::vec3 position, glm::vec3 velocity)
		{
			Position = position;
			Velocity = velocity;

			Front = glm::normalize(Velocity);
			Right = glm::normalize(glm::cross(Front, WorldUp));
			Up = glm::normalize(glm::cross(Right, Front));
			WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		}

		void SetVelocity(glm::vec3 velocity)
		{
			Velocity = velocity;

			Front = glm::normalize(velocity);
			Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = glm::normalize(glm::cross(Right, Front));
		}
	};

	glm::vec3 WrapPositionToBox(glm::vec3 position);
	float WrapValueToBox(float value, Axis axis);
	glm::vec3 GetRandomStartingPosition();
	glm::vec3 GetRandomStartingVelocity();
	glm::vec3 GetCohesionVelocity(int index);
	glm::vec3 GetSeparationVelocity(int index);
	glm::vec3 GetAlignmentVelocity(int index);
	glm::vec3 GetWallAvoidanceVelocity(int index);
	bool IsWithinBox(glm::vec3 point);

	const double PI = 3.14159265358979323846;
	const float BOX_WIDTH = 40.0f;
	const float BOX_HALFWIDTH = BOX_WIDTH / 2;
	const float BOX_HEIGHT = 20.0f;
	const float BOX_HALFHEIGHT = BOX_HEIGHT / 2;
	const float BOX_LENGTH = 20.0f;
	const float BOX_HALFLENGTH = BOX_LENGTH / 2;
	const unsigned int BOID_AMOUNT = 200;

	float AngleOfSight = 0.25f;
	float MoveSpeed = 3.0f;
	float SightRange = 5;
	float CohesionFactor = 0.01f;
	float AlignmentFactor = 0.125f;
	float SeparationFactor = 0.015;
	float WallAvoidanceFactor = 30.0f;

	float m_BoxVertices[] = {
		-1.0f, -1.0f, -1.0f, // 0 - left bottom back
		 1.0f, -1.0f, -1.0f, // 1 - right bottom back
		 1.0f,  1.0f, -1.0f, // 2 - right top back
		-1.0f,  1.0f, -1.0f, // 3 - left top back
		-1.0f, -1.0f,  1.0f, // 4 - left bottom front
		 1.0f, -1.0f,  1.0f, // 5 - right bottom front
		 1.0f,  1.0f,  1.0f, // 6 - right top front
		-1.0f,  1.0f,  1.0f  // 7 - left top front
	};
	unsigned int m_BoxIndices[] = {
		// back face
		0, 1,
		1, 2,
		2, 3,
		3, 0,

		4, 5,
		5, 6,
		6, 7,
		7, 4,

		0, 4,
		1, 5,
		2, 6,
		3, 7,
	};

	float m_BoidVertices[] = {
		// Tip of the cone
		 0.0f, 1.0f,  0.0f,   // Vertex 0: tip

		 // Base vertices (square)
		 -0.5f, 0.0f, -0.5f,   // Vertex 1: back-left
		  0.5f, 0.0f, -0.5f,   // Vertex 2: back-right
		  0.5f, 0.0f,  0.5f,   // Vertex 3: front-right
		 -0.5f, 0.0f,  0.5f    // Vertex 4: front-left
	};
	unsigned int m_BoidIndices[] = {
		// Side triangles (tip to base)
		0, 1, 2,   // Tip to back-left and back-right
		0, 2, 3,   // Tip to back-right and front-right
		0, 3, 4,   // Tip to front-right and front-left
		0, 4, 1,   // Tip to front-left and back-left

		// Base (two triangles)
		1, 2, 3,   // Base triangle 1
		1, 3, 4    // Base triangle 2
	};

	std::vector<float> m_LineVertices;
	std::vector<unsigned int> m_LineIndices;

	unsigned int m_BoidVao, m_BoxVao, m_LineVao, m_LineVbo;
	std::unique_ptr<Shader> m_BoidShader, m_BoxShader, m_LineShader;
	float m_ScrWidth, m_ScrHeight, m_Aspect;
	std::vector<Boid> m_Boids;

	void Init(int screenWidth, int screenHeight)
	{
		m_ScrWidth = screenWidth;
		m_ScrHeight = screenHeight;
		m_Aspect = m_ScrWidth / m_ScrHeight;

		// boid definition
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			glm::vec3 pos = GetRandomStartingPosition();
			glm::vec3 vel = GetRandomStartingVelocity();
			Boid a(pos, vel);
			m_Boids.push_back(a);
		}

		// boid render
		SetArrayBuffer(&m_BoidVao, m_BoidVertices, sizeof(m_BoidVertices), m_BoidIndices, sizeof(m_BoidIndices));

		//unsigned int boidVbo;
		//unsigned int boidEbo;

		//glGenVertexArrays(1, &m_BoidVao);
		//glGenBuffers(1, &boidVbo);
		//glGenBuffers(1, &boidEbo);

		//glBindVertexArray(m_BoidVao);

		//glBindBuffer(GL_ARRAY_BUFFER, boidVbo);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(m_BoidVertices), m_BoidVertices, GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boidEbo);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_BoidIndices), m_BoidIndices, GL_STATIC_DRAW);

		//// position attribute
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);

		m_BoidShader = std::make_unique<Shader>("BoidShader.vert", "BoidShader.frag");
		m_BoidShader->use();
		m_BoidShader->setMat4("model", glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

		// box render
		SetArrayBuffer(&m_BoxVao, m_BoxVertices, sizeof(m_BoxVertices), m_BoxIndices, sizeof(m_BoxIndices));

		m_BoxShader = std::make_unique<Shader>("BoxShader.vert", "BoxShader.frag");

		// line rendering
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			//v1
			m_LineVertices.push_back(0);
			m_LineVertices.push_back(0);
			m_LineVertices.push_back(0);
			//v2
			m_LineVertices.push_back(0);
			m_LineVertices.push_back(0);
			m_LineVertices.push_back(0);

			m_LineIndices.push_back(i * 2);
			m_LineIndices.push_back(i * 2 + 1);
		}

		unsigned int ebo;

		glGenVertexArrays(1, &m_LineVao);
		glGenBuffers(1, &m_LineVbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(m_LineVao);

		glBindBuffer(GL_ARRAY_BUFFER, m_LineVbo);
		glBufferData(GL_ARRAY_BUFFER, m_LineVertices.size() * sizeof(float), m_LineVertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_LineIndices.size() * sizeof(unsigned int), m_LineIndices.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		m_LineShader = std::make_unique<Shader>("LineShader.vert", "LineShader.frag");
	}

	void Update(float deltaTime)
	{
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			glm::vec3 velocity = glm::vec3(0);
			velocity += GetCohesionVelocity(i);
			velocity += GetSeparationVelocity(i);
			velocity += GetAlignmentVelocity(i);
			velocity += GetWallAvoidanceVelocity(i);

			m_Boids[i].SetVelocity(glm::normalize(m_Boids[i].Velocity + velocity) * MoveSpeed);

			m_Boids[i].Position += m_Boids[i].Velocity * deltaTime;
			//m_Boids[i].Position = WrapPositionToBox(m_Boids[i].Position);

			// update line vertices
			glm::vec3 direction = m_Boids[i].Velocity * 1.0f;
			m_LineVertices[i * 6 + 0] = m_Boids[i].Position.x;
			m_LineVertices[i * 6 + 1] = m_Boids[i].Position.y;
			m_LineVertices[i * 6 + 2] = m_Boids[i].Position.z;
			m_LineVertices[i * 6 + 3] = m_Boids[i].Position.x + direction.x;
			m_LineVertices[i * 6 + 4] = m_Boids[i].Position.y + direction.y;
			m_LineVertices[i * 6 + 5] = m_Boids[i].Position.z + direction.z;
		}
	}

	void Render(Camera camera)
	{
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), m_Aspect, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		// boids
		glBindVertexArray(m_BoidVao);

		m_BoidShader->use();
		m_BoidShader->setMat4("projection", projection);
		m_BoidShader->setMat4("view", view);

		glm::mat4 agentRotScale = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		for (GLubyte i = 0; i < BOID_AMOUNT; i++)
		{
			m_BoidShader->setMat4("model", glm::translate(agentRotScale, m_Boids[i].Position));
			glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_INT, 0);
		}

		// bounding box
		glBindVertexArray(m_BoxVao);

		m_BoxShader->use();
		m_BoxShader->setMat4("projection", projection);
		m_BoxShader->setMat4("view", view);
		glm::mat4 boxRotScale = glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(BOX_HALFWIDTH, BOX_HALFHEIGHT, BOX_HALFLENGTH)),
			glm::radians(0.0f), glm::vec3(1, 0, 0));
		m_BoxShader->setMat4("model", boxRotScale);

		glDrawElements(GL_LINES, 12 * 2, GL_UNSIGNED_INT, 0);

		// lines
		glBindVertexArray(m_LineVao);

		glBindBuffer(GL_ARRAY_BUFFER, m_LineVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_LineVertices.size() * sizeof(float), m_LineVertices.data());

		m_LineShader->use();
		m_LineShader->setMat4("projection", projection);
		m_LineShader->setMat4("view", view);
		m_LineShader->setMat4("model", glm::mat4(1.0f));

		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	}

	void ResetBoids()
	{
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			m_Boids[i].Position = GetRandomStartingPosition();
			m_Boids[i].SetVelocity(GetRandomStartingVelocity());
		}
	}

	glm::vec3 WrapPositionToBox(glm::vec3 position)
	{
		position.x = WrapValueToBox(position.x, XAXIS);
		position.y = WrapValueToBox(position.y, YAXIS);
		position.z = WrapValueToBox(position.z, ZAXIS);

		return position;
	}

	float WrapValueToBox(float value, Axis axis)
	{
		switch (axis)
		{
		case XAXIS:
			value += BOX_HALFWIDTH;
			value = fmod((fmod(value, BOX_WIDTH) + BOX_WIDTH), BOX_WIDTH);
			value -= BOX_HALFWIDTH;
			return value;
		case YAXIS:
			value += BOX_HALFHEIGHT;
			value = fmod((fmod(value, BOX_HEIGHT) + BOX_HEIGHT), BOX_HEIGHT);
			value -= BOX_HALFHEIGHT;
			return value;
		case ZAXIS:
			value += BOX_HALFLENGTH;
			value = fmod((fmod(value, BOX_LENGTH) + BOX_LENGTH), BOX_LENGTH);
			value -= BOX_HALFLENGTH;
			return value;
		default:
			break;
		}
	}

	bool IsWithinBox(glm::vec3 point)
	{
		return abs(point.x) < BOX_HALFWIDTH && abs(point.y) < BOX_HALFHEIGHT && abs(point.z) < BOX_HALFLENGTH;
	}

	glm::vec3 GetRandomStartingPosition()
	{
		float randomX = WrapValueToBox(rand(), XAXIS);
		float randomY = WrapValueToBox(rand(), YAXIS);
		float randomZ = WrapValueToBox(rand(), ZAXIS);
		return glm::vec3(randomX, randomY, randomZ);
	}
	glm::vec3 GetRandomStartingVelocity()
	{
		float randomX = rand() - ((float)RAND_MAX / 2);
		if (randomX >= 0)
			return glm::vec3(1, 0, 0);
		return glm::vec3(-1, 0, 0);
	}

	glm::vec3 GetCohesionVelocity(int index)
	{
		glm::vec3 pseudoCenter(0);
		int groupAmount = 0;
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			if (i == index)
			{
				continue;
			}

			glm::vec3 distance = m_Boids[i].Position - m_Boids[index].Position;

			if (glm::dot(m_Boids[index].Front, distance) <= AngleOfSight)
			{
				continue;
			}

			if (glm::length(distance) > SightRange)
			{
				continue;
			}

			pseudoCenter += m_Boids[i].Position;
			groupAmount++;
		}

		if (groupAmount == 0)
		{
			return glm::vec3(0);
		}

		pseudoCenter /= groupAmount;

		glm::vec3 resultVelocity = (pseudoCenter - m_Boids[index].Position) * CohesionFactor;
		return resultVelocity;
	}

	glm::vec3 GetSeparationVelocity(int index)
	{
		glm::vec3 velocity(0);
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			if (i == index)
			{
				continue;
			}

			glm::vec3 distance = m_Boids[i].Position - m_Boids[index].Position;

			if (glm::dot(m_Boids[index].Front, distance) <= AngleOfSight)
			{
				continue;
			}

			float length = glm::length(distance);
			if (length > SightRange)
			{
				continue;
			}

			float influence = 1 - length / SightRange;
			velocity -= distance * influence;
		}

		return velocity * SeparationFactor;
	}

	glm::vec3 GetAlignmentVelocity(int index)
	{
		glm::vec3 velocityAvg(0);
		int groupAmount = 0;
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			if (i == index)
			{
				continue;
			}

			glm::vec3 distance = m_Boids[i].Position - m_Boids[index].Position;

			if (glm::dot(m_Boids[index].Front, distance) <= AngleOfSight)
			{
				continue;
			}

			if (glm::length(distance) > SightRange)
			{
				continue;
			}

			velocityAvg += m_Boids[i].Velocity;
			groupAmount++;
		}

		if (groupAmount == 0)
		{
			return glm::vec3(0);
		}

		velocityAvg /= groupAmount;

		glm::vec3 resultVelocity = velocityAvg * AlignmentFactor;
		return resultVelocity;
	}

	glm::vec3 GetWallAvoidanceVelocity(int index)
	{
		glm::vec3 pos = m_Boids[index].Position;
		glm::vec3 front = m_Boids[index].Front;

		float rayLength = 3;
		glm::vec3 frontPoint = pos + front * rayLength;

		if (IsWithinBox(frontPoint))
		{
			return glm::vec3(0);
		}

		int maxPoints = 30;
		float turnFraction = 0.1f;

		float bestDot = -1;
		glm::vec3 bestRay = -front;

		for (size_t i = 0; i < maxPoints; i++)
		{
			// run through spiral
			float t = (float)i / (maxPoints - 1);
			float polarFactor = acos(1 - 2 * t);
			float azimuthalFactor = 2 * PI * turnFraction * i;

			glm::vec3 ray(
				rayLength * sin(polarFactor) * cos(azimuthalFactor),
				rayLength * cos(polarFactor),
				rayLength * sin(polarFactor) * sin(azimuthalFactor)
			);

			if (IsWithinBox(pos + ray))
			{
				float dot = glm::dot(front, ray);
				if (dot > bestDot)
				{
					bestDot = dot;
					bestRay = ray;
				}
			}
		}

		return bestRay * WallAvoidanceFactor;
	}
}