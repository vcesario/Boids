#pragma once

#include <iostream>

#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "CameraController.h"

namespace Ui
{
	void Init(GLFWwindow* window);
	void Terminate();
	void Render(CameraController& camController);
	bool IsHovered();
}