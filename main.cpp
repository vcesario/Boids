#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "imgui/imgui.h"

#include "Timer.h"
#include "Ui.h"
#include "BoidManager.h"
#include "Input.h"
#include "Camera.h"
#include "CameraController.h"

const unsigned int SCR_WIDTH = 1920 / 2;
const unsigned int SCR_HEIGHT = 1080 / 2;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Boids", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(0); // vsync

	Timer::Init();
	BoidManager::Init(SCR_WIDTH, SCR_HEIGHT);
	Input::Init(window);
	Ui::Init(window);

	Camera camera(glm::vec3(0.0f, 0.0f, 40.0f));
	CameraController camController(&camera);
	
	while (!glfwWindowShouldClose(window))
	{
		Timer::NewFrame();

		Input::Update();

		if (Input::IsActionActive(Input::EXIT))
		{
			glfwSetWindowShouldClose(window, true);
			continue;
		}

		camController.Update();
		BoidManager::Update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		BoidManager::Render(camera);

		Ui::Render(camController);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Ui::Terminate();

	glfwTerminate();
	return 0;
}