#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "BoidManager.h"

namespace BoidManager
{
	const float BOX_WIDTH = 20.0f;
	const float BOX_HALFWIDTH = BOX_WIDTH / 2;
	const float BOX_HEIGHT = 20.0f;
	const float BOX_HALFHEIGHT = BOX_HEIGHT / 2;
	const float BOX_LENGTH = 20.0f;
	const float BOX_HALFLENGTH = BOX_LENGTH / 2;
	const GLubyte BOID_AMOUNT = 20;
	
	float MoveSpeed = 0.2f;
	float CohesionFactor = 0.01f;
	float CohesionDistance = 1;
	float AlignmentFactor = 0.125f;
	float AlignmentDistance = 50;
	float SeparationFactor = 1;
	float SeparationDistance = 1;

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

	unsigned int m_BoidVao, m_BoxVao;
	std::unique_ptr<Shader> m_BoidShader, m_BoxShader;
	float m_ScrWidth, m_ScrHeight, m_Aspect;
	std::vector<Boid> m_Boids;

	glm::vec3 WrapPositionToBox(glm::vec3 position);
	float WrapValueToBox(float value, Axis axis);
	glm::vec3 GetRandomPositionWithinBox();
	glm::vec3 GetCohesionVelocity(int index);
	glm::vec3 GetSeparationVelocity(int index);
	glm::vec3 GetAlignmentVelocity(int index);

	void Init(int screenWidth, int screenHeight)
	{
		m_ScrWidth = screenWidth;
		m_ScrHeight = screenHeight;
		m_Aspect = m_ScrWidth / m_ScrHeight;

		// boid definition
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			glm::vec3 pos = GetRandomPositionWithinBox();
			Boid a(pos, glm::vec3(0));
			m_Boids.push_back(a);
		}

		// boid render
		unsigned int boidVbo;
		unsigned int boidEbo;

		glGenVertexArrays(1, &m_BoidVao);
		glGenBuffers(1, &boidVbo);
		glGenBuffers(1, &boidEbo);

		glBindVertexArray(m_BoidVao);

		glBindBuffer(GL_ARRAY_BUFFER, boidVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_BoidVertices), m_BoidVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boidEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_BoidIndices), m_BoidIndices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		m_BoidShader = std::make_unique<Shader>("BoidShader.vert", "BoidShader.frag");
		m_BoidShader->use();
		m_BoidShader->setMat4("model", glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

		// box render
		unsigned int boxVbo;
		unsigned int boxEbo;

		glGenVertexArrays(1, &m_BoxVao);
		glGenBuffers(1, &boxVbo);
		glGenBuffers(1, &boxEbo);

		glBindVertexArray(m_BoxVao);

		glBindBuffer(GL_ARRAY_BUFFER, boxVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_BoxVertices), m_BoxVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_BoxIndices), m_BoxIndices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		m_BoxShader = std::make_unique<Shader>("BoxShader.vert", "BoxShader.frag");
	}

	void Update(float deltaTime)
	{
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			glm::vec3 cohesion = GetCohesionVelocity(i);
			glm::vec3 separation = GetSeparationVelocity(i);
			glm::vec3 alignment = GetAlignmentVelocity(i);
			m_Boids[i].Velocity += cohesion + separation + alignment;

			m_Boids[i].Position += m_Boids[i].Velocity * MoveSpeed;
			m_Boids[i].Position = WrapPositionToBox(m_Boids[i].Position);
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
		glm::mat4 boxRotScale = glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(BOX_HALFWIDTH, BOX_HALFHEIGHT, BOX_HALFLENGTH)),
			glm::radians(0.0f), glm::vec3(1, 0, 0));
		m_BoxShader->setMat4("model", boxRotScale);
		m_BoxShader->setMat4("projection", projection);
		m_BoxShader->setMat4("view", view);

		glDrawElements(GL_LINES, 12 * 2, GL_UNSIGNED_INT, 0);
	}

	void ResetBoids()
	{
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			m_Boids[i].Velocity = glm::vec3(0);
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

	glm::vec3 GetRandomPositionWithinBox()
	{
		float randomX = WrapValueToBox(rand(), XAXIS);
		float randomY = WrapValueToBox(rand(), YAXIS);
		float randomZ = WrapValueToBox(rand(), ZAXIS);
		return glm::vec3(randomX, randomY, randomZ);
	}

	glm::vec3 GetCohesionVelocity(int index)
	{
		glm::vec3 pseudoCenter(0);
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			if (i == index)
			{
				continue;
			}

			glm::vec3 distance = m_Boids[i].Position - m_Boids[index].Position;
			if (glm::length(distance) > CohesionDistance)
			{
				continue;
			}

			pseudoCenter += m_Boids[i].Position;
		}
		pseudoCenter /= (BOID_AMOUNT - 1);

		glm::vec3 resultVelocity = (pseudoCenter - m_Boids[index].Position) * CohesionFactor;

		return resultVelocity;
	}

	glm::vec3 GetSeparationVelocity(int index)
	{
		glm::vec3 resultVelocity(0);
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			if (i == index)
			{
				continue;
			}

			glm::vec3 distance = m_Boids[i].Position - m_Boids[index].Position;
			if (glm::length(distance) > SeparationDistance)
			{
				continue;
			}

			resultVelocity -= distance;
		}

		return resultVelocity * SeparationFactor;
	}

	glm::vec3 GetAlignmentVelocity(int index)
	{
		glm::vec3 pseudoVelocityAvg(0);
		for (size_t i = 0; i < BOID_AMOUNT; i++)
		{
			if (i == index)
			{
				continue;
			}

			glm::vec3 distance = m_Boids[i].Position - m_Boids[index].Position;
			if (glm::length(distance) > AlignmentDistance)
			{
				continue;
			}

			pseudoVelocityAvg += m_Boids[i].Velocity;
		}
		pseudoVelocityAvg /= (BOID_AMOUNT - 1);

		glm::vec3 resultVelocity = (pseudoVelocityAvg - m_Boids[index].Velocity) * AlignmentFactor;

		return resultVelocity;
	}
}