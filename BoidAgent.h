#pragma once
#include "Shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace BoidAgent
{
	void WrapPositionToBox(glm::vec3* position);

	const float BOX_WIDTH = 5.33f;
	const float BOX_HALFWIDTH = BOX_WIDTH / 2;
	const float BOX_HEIGHT = 4.0f;
	const float BOX_HALFHEIGHT = BOX_HEIGHT / 2;
	const float BOX_LENGTH = 3.0f;
	const float BOX_HALFLENGTH = BOX_LENGTH / 2;
	const float MOVE_SPEED = 3.29f;

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

	float m_AgentVertices[] = {
		// Tip of the cone
		 0.0f, 1.0f,  0.0f,   // Vertex 0: tip

		 // Base vertices (square)
		 -0.5f, 0.0f, -0.5f,   // Vertex 1: back-left
		  0.5f, 0.0f, -0.5f,   // Vertex 2: back-right
		  0.5f, 0.0f,  0.5f,   // Vertex 3: front-right
		 -0.5f, 0.0f,  0.5f    // Vertex 4: front-left
	};
	unsigned int m_AgentIndices[] = {
		// Side triangles (tip to base)
		0, 1, 2,   // Tip to back-left and back-right
		0, 2, 3,   // Tip to back-right and front-right
		0, 3, 4,   // Tip to front-right and front-left
		0, 4, 1,   // Tip to front-left and back-left

		// Base (two triangles)
		1, 2, 3,   // Base triangle 1
		1, 3, 4    // Base triangle 2
	};

	unsigned int m_AgentVao, m_BoxVao;
	std::unique_ptr<Shader> m_AgentShader, m_BoxShader;
	float m_ScrWidth, m_ScrHeight;

	glm::vec3 m_AgentPositions[] = {
		glm::vec3(0, 0, 0),
		glm::vec3(0, 2, 1),
		glm::vec3(3, 0, 0)
	};

	void Init(int screenWidth, int screenHeight)
	{
		m_ScrWidth = screenWidth;
		m_ScrHeight = screenHeight;

		// agent
		unsigned int agentVbo;
		unsigned int agentEbo;

		glGenVertexArrays(1, &m_AgentVao);
		glGenBuffers(1, &agentVbo);
		glGenBuffers(1, &agentEbo);

		glBindVertexArray(m_AgentVao);

		glBindBuffer(GL_ARRAY_BUFFER, agentVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_AgentVertices), m_AgentVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, agentEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_AgentIndices), m_AgentIndices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		m_AgentShader = std::make_unique<Shader>("AgentShader.vert", "AgentShader.frag");
		m_AgentShader->use();
		m_AgentShader->setMat4("model", glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

		// box
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
		m_AgentPositions[0] += glm::vec3(0.2f, -1, -1) * deltaTime * MOVE_SPEED;
		WrapPositionToBox(&m_AgentPositions[0]);
	}

	void WrapPositionToBox(glm::vec3* position)
	{
		if (position->x > BOX_HALFWIDTH
			|| position->x < -BOX_HALFWIDTH)
		{
			position->x = remainder(position->x, BOX_WIDTH);
		}

		if (position->y > BOX_HALFHEIGHT
			|| position->y < -BOX_HALFHEIGHT)
		{
			position->y = remainder(position->y, BOX_HEIGHT);
		}

		if (position->z > BOX_HALFLENGTH
			|| position->z < -BOX_HALFLENGTH)
		{
			position->z = remainder(position->z, BOX_LENGTH);
		}
	}

	void Render(Camera camera)
	{
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), m_ScrWidth / m_ScrHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		// agents
		glBindVertexArray(m_AgentVao);

		m_AgentShader->use();
		m_AgentShader->setMat4("projection", projection);
		m_AgentShader->setMat4("view", view);

		glm::mat4 agentRotScale = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		for (GLubyte i = 0; i < m_AgentPositions->length(); i++)
		{
			m_AgentShader->setMat4("model", glm::translate(agentRotScale, m_AgentPositions[i]));
			glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_INT, 0);
		}

		// bounding box
		glBindVertexArray(m_BoxVao);

		m_BoxShader->use();
		glm::mat4 boxRotScale = glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(BOX_WIDTH / 2, BOX_HEIGHT / 2, BOX_LENGTH / 2)),
			glm::radians(0.0f), glm::vec3(1, 0, 0));
		m_BoxShader->setMat4("model", boxRotScale);
		m_BoxShader->setMat4("projection", projection);
		m_BoxShader->setMat4("view", view);

		glDrawElements(GL_LINES, 12 * 2, GL_UNSIGNED_INT, 0);
	}
}