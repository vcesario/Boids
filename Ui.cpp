//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/string_cast.hpp>

#include "Ui.h"
#include "Useful.h"

GLFWwindow* m_Window;
const ImVec2 m_InspectorWindowSize(300, 250);
const ImVec2 m_InspectorWindowPos(10, 100);

void Ui::Init(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(/*"#version 130"*/);

	m_Window = window;
}

void Ui::Terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool Ui::IsHovered()
{
	return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
}

void Ui::Render(CameraController& camController)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;

	ImGui::Begin("Inspector", NULL, windowFlags);

	ImGui::SetWindowSize(m_InspectorWindowSize, ImGuiCond_Once);
	ImGui::SetWindowPos(m_InspectorWindowPos, ImGuiCond_Once);

	// VALUES
	// 
	// CAMERA
	// (readonly) Cam Position
	// (readonly) Cam Rotation
	// (readonly) Cam Zoom
	// Pan Sensitivity
	// Rotate Sensitivity
	// Movement Speed
	//
	// BOID MANAGER
	// CohesionFactor
	// CohesionDistance
	// SeparationFactor
	// SeparationDistance
	// AlignmentFactor
	// AlignmentDistance
	// ---

	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextDisabled("Position: %s", ToString(camController.MasterCam->Position).c_str());
		ImGui::TextDisabled("Rotation: %s", YawPitchToString(camController.MasterCam->Yaw, camController.MasterCam->Pitch).c_str());
		ImGui::TextDisabled("Zoom: %.2f", camController.MasterCam->Zoom);
		ImGui::SliderFloat("Move Speed", &camController.MovementSpeed, 1.0f, 20.0f);
		ImGui::SliderFloat("Rotate Speed", &camController.RotateSensitivity, 0.1f, 1.0f);
		ImGui::SliderFloat("Pan Speed", &camController.PanSensitivity, 0.001f, 0.5f);
	}

	ImGui::End();

	//ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}