#include <iostream>
#include <unordered_map>

#include "Input.h"
#include "Globals.h"

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
	std::unordered_map<Input_Action, int> m_InactiveActions;

	GLFWwindow* m_Window;

	void Init(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, HandleKeyCallback);
		glfwSetMouseButtonCallback(window, HandleMouseButtonCallback);
		glfwSetScrollCallback(window, HandleMouseScrollCallback);

		glfwGetCursorPos(window, &m_XCurrent, &m_YCurrent);
		m_XPrevious = m_XCurrent;
		m_YPrevious = m_YCurrent;

		m_Window = window;
	}

	void Update()
	{
		m_XPrevious = m_XCurrent;
		m_YPrevious = m_YCurrent;
		glfwGetCursorPos(m_Window, &m_XCurrent, &m_YCurrent);

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
				//m_ActiveActions.erase(EXIT);
				m_InactiveActions[EXIT] = Globals::FRAME_COUNT;
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
				//m_ActiveActions.erase(FORWARD);
				m_InactiveActions[FORWARD] = Globals::FRAME_COUNT;
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
				//m_ActiveActions.erase(BACKWARD);
				m_InactiveActions[BACKWARD] = Globals::FRAME_COUNT;
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
				//m_ActiveActions.erase(PANLEFT);
				m_InactiveActions[PANLEFT] = Globals::FRAME_COUNT;
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
				//m_ActiveActions.erase(PANRIGHT);
				m_InactiveActions[PANRIGHT] = Globals::FRAME_COUNT;
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
				m_InactiveActions[LEFTDRAG] = Globals::FRAME_COUNT;
				//m_ActiveActions.erase(LEFTDRAG);
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
				m_InactiveActions[RIGHTDRAG] = Globals::FRAME_COUNT;
				//m_ActiveActions.erase(RIGHTDRAG);
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
				m_InactiveActions[RESETFOCUS] = Globals::FRAME_COUNT;
				//m_ActiveActions.erase(RESETFOCUS);
			}
		}
	}

	void HandleMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		m_ActiveActions[ZOOM] = Globals::FRAME_COUNT;
		m_InactiveActions[ZOOM] = Globals::FRAME_COUNT;
		ScrollYDiff = yoffset;
	}

	bool IsActionActive(Input_Action action)
	{
		// optimize: initialize the maps to always contain all actions from the getgo so I don't need to check if they exist
		auto activeFrame = m_ActiveActions.find(action);
		if (activeFrame == m_ActiveActions.end())
		{
			return false;
		}

		auto inactiveFrame = m_InactiveActions.find(action);
		if (inactiveFrame == m_InactiveActions.end())
		{
			return true;
		}

		return  activeFrame->second > inactiveFrame->second;
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

	bool IsActionUp(Input_Action action)
	{
		auto frame = m_InactiveActions.find(action);
		if (frame == m_InactiveActions.end())
		{
			return false;
		}
		return (frame->second + 1) == Globals::FRAME_COUNT;
	}
}