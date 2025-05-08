#include "Shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace BoidAgent
{
	float vertices[] = {
		// Tip of the cone
		 0.0f, 1.0f,  0.0f,   // Vertex 0: tip

		 // Base vertices (square)
		 -0.5f, 0.0f, -0.5f,   // Vertex 1: back-left
		  0.5f, 0.0f, -0.5f,   // Vertex 2: back-right
		  0.5f, 0.0f,  0.5f,   // Vertex 3: front-right
		 -0.5f, 0.0f,  0.5f    // Vertex 4: front-left
	};

	unsigned int indices[] = {
		// Side triangles (tip to base)
		0, 1, 2,   // Tip to back-left and back-right
		0, 2, 3,   // Tip to back-right and front-right
		0, 3, 4,   // Tip to front-right and front-left
		0, 4, 1,   // Tip to front-left and back-left

		// Base (two triangles)
		1, 2, 3,   // Base triangle 1
		1, 3, 4    // Base triangle 2
	};

	unsigned int vao;
	std::unique_ptr<Shader> shader;
	float m_ScrWidth, m_ScrHeight;

	void Init(int screenWidth, int screenHeight)
	{
		m_ScrWidth = screenWidth;
		m_ScrHeight = screenHeight;

		unsigned int vbo;
		unsigned int ebo;

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		shader = std::make_unique<Shader>("AgentShader.vert", "AgentShader.frag");
		
		shader->use();

		shader->setMat4("model", glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		//shader->setMat4("view", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)));
		//shader->setMat4("projection", glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f));
	}

	void Render(Camera camera)
	{
		shader->use();

		shader->setMat4("projection", glm::perspective(glm::radians(camera.Zoom), m_ScrWidth / m_ScrHeight, 0.1f, 100.0f));
		shader->setMat4("view", camera.GetViewMatrix());

		float time = (float)glfwGetTime();
		float speed = 25;
		//shader->setMat4("model", glm::rotate(glm::mat4(1.0f), glm::radians(time) * speed, glm::vec3(1.0f, 0.0f, 0.0f)));

		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_INT, 0);
	}
}