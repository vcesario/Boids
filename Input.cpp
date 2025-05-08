#include "Input.h"

#include <iostream>

namespace Input
{
	void HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void HandleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	
	bool ExitActionPerformed = false;
	double XDiff = 0, YDiff = 0;
	
	bool m_IsDragging = false;
	double m_XPrevious, m_YPrevious, m_XCurrent, m_YCurrent;

	void Init(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, HandleKeyCallback);
		glfwSetMouseButtonCallback(window, HandleMouseButtonCallback);
	}

	void Process(GLFWwindow* window)
	{
		if (m_IsDragging)
		{
			m_XPrevious = m_XCurrent;
			m_YPrevious = m_YCurrent;
			glfwGetCursorPos(window, &m_XCurrent, &m_YCurrent);

			XDiff = m_XCurrent - m_XPrevious;
			YDiff = m_YCurrent - m_YPrevious;
		}
	}

	void HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			if (action == GLFW_PRESS)
			{
				ExitActionPerformed = true;
			}
			if (action == GLFW_RELEASE)
			{
				ExitActionPerformed = false;
			}
		}
	}

	void HandleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			if (action == GLFW_PRESS)
			{
				m_IsDragging = true;

				glfwGetCursorPos(window, &m_XCurrent, &m_YCurrent);
				m_XPrevious = m_XCurrent;
				m_YPrevious = m_YCurrent;
			}
			if (action == GLFW_RELEASE)
			{
				m_IsDragging = false;
			}
		}
	}
}