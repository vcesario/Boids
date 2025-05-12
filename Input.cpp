#include "Input.h"
#include "Globals.h"

#include <iostream>
#include <unordered_map>

namespace Input
{
	void HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void HandleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void HandleMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	bool BackwardActionActive = false;
	bool PanLeftActionActive = false, PanRightActionActive = false;
	double XDiff = 0, YDiff = 0;
	double ScrollYDiff = 0;

	double m_XPrevious, m_YPrevious, m_XCurrent, m_YCurrent;

	std::unordered_map<Input_Action, int> m_ActiveActions;

	void Init(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, HandleKeyCallback);
		glfwSetMouseButtonCallback(window, HandleMouseButtonCallback);
		glfwSetScrollCallback(window, HandleMouseScrollCallback);

		glfwGetCursorPos(window, &m_XCurrent, &m_YCurrent);
		m_XPrevious = m_XCurrent;
		m_YPrevious = m_YCurrent;
	}

	void Process(GLFWwindow* window)
	{
		m_XPrevious = m_XCurrent;
		m_YPrevious = m_YCurrent;
		glfwGetCursorPos(window, &m_XCurrent, &m_YCurrent);

		XDiff = m_XCurrent - m_XPrevious;
		YDiff = m_YCurrent - m_YPrevious;
	}

	void HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			if (action == GLFW_PRESS)
			{
				m_ActiveActions[EXIT] = Globals::FRAME_COUNT;
			}
			else if (action == GLFW_RELEASE)
			{
				m_ActiveActions.erase(EXIT);
			}
		}

		if (key == GLFW_KEY_W)
		{
			if (action == GLFW_PRESS)
			{
				m_ActiveActions[FORWARD] = Globals::FRAME_COUNT;
			}
			else if (action == GLFW_RELEASE)
			{
				m_ActiveActions.erase(FORWARD);
			}
		}
		else if (key == GLFW_KEY_S)
		{
			if (action == GLFW_PRESS)
			{
				m_ActiveActions[BACKWARD] = Globals::FRAME_COUNT;
			}
			else if (action == GLFW_RELEASE)
			{
				m_ActiveActions.erase(BACKWARD);
			}
		}

		if (key == GLFW_KEY_A)
		{
			if (action == GLFW_PRESS)
			{
				m_ActiveActions[PANLEFT] = Globals::FRAME_COUNT;
			}
			else if (action == GLFW_RELEASE)
			{
				m_ActiveActions.erase(PANLEFT);
			}
		}
		else if (key == GLFW_KEY_D)
		{
			if (action == GLFW_PRESS)
			{
				m_ActiveActions[PANRIGHT] = Globals::FRAME_COUNT;
			}
			else if (action == GLFW_RELEASE)
			{
				m_ActiveActions.erase(PANRIGHT);
			}
		}
	}

	void HandleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			if (action == GLFW_PRESS)
			{
				m_ActiveActions[LEFTDRAG] = Globals::FRAME_COUNT;

				glfwGetCursorPos(window, &m_XCurrent, &m_YCurrent);
				m_XPrevious = m_XCurrent;
				m_YPrevious = m_YCurrent;
			}
			if (action == GLFW_RELEASE)
			{
				m_ActiveActions.erase(LEFTDRAG);
			}
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			if (action == GLFW_PRESS)
			{
				m_ActiveActions[RIGHTDRAG] = Globals::FRAME_COUNT;

				glfwGetCursorPos(window, &m_XCurrent, &m_YCurrent);
				m_XPrevious = m_XCurrent;
				m_YPrevious = m_YCurrent;
			}
			if (action == GLFW_RELEASE)
			{
				m_ActiveActions.erase(RIGHTDRAG);
			}
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			if (action == GLFW_PRESS)
			{
				m_ActiveActions[RESETFOCUS] = Globals::FRAME_COUNT;
			}
			if (action == GLFW_RELEASE)
			{
				m_ActiveActions.erase(RESETFOCUS);
			}
		}
	}

	void HandleMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		m_ActiveActions[ZOOM] = Globals::FRAME_COUNT;
		ScrollYDiff = yoffset;
	}

	bool IsActionActive(Input_Action action)
	{
		return m_ActiveActions.find(action) != m_ActiveActions.end();
	}

	bool IsActionDown(Input_Action action)
	{
		auto frame = m_ActiveActions.find(action);
		if (frame == m_ActiveActions.end())
		{
			return false;
		}
		return (frame->second + 1) == Globals::FRAME_COUNT;
	}
}