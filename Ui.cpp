#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Ui.h"
#include "Useful.h"
#include "BoidManager.h"

GLFWwindow* m_Window;
const ImVec2 m_InspectorWindowSize(280, 400);
const ImVec2 m_InspectorWindowPos(10, 100);

void LoadValues();
void SaveValues();

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

	if (ImGui::CollapsingHeader("Boid Manager", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextDisabled("Boid amount: %d", BoidManager::BOID_AMOUNT);
		ImGui::DragFloat("MoveSpeed", &BoidManager::MoveSpeed, 0.001f, 0, 10);
		ImGui::DragFloat("Cohesion Factor", &BoidManager::CohesionFactor, 0.001f, 0, 10);
		ImGui::SliderFloat("Cohesion Distance", &BoidManager::CohesionDistance, 0, 50);
		ImGui::DragFloat("Separation Factor", &BoidManager::SeparationFactor, 0.001f, 0, 10);
		ImGui::SliderFloat("Separation Distance", &BoidManager::SeparationDistance, 0, 50);
		ImGui::DragFloat("Alignment Factor", &BoidManager::AlignmentFactor, 0.001f, 0, 10);
		ImGui::SliderFloat("Alignment Distance", &BoidManager::AlignmentDistance, 0, 50);
	}

	if (ImGui::Button("Load values"))
	{
		LoadValues();
	}

	ImGui::SameLine();
	if (ImGui::Button("Save values"))
	{
		SaveValues();
	}

	ImGui::SameLine();
	if (ImGui::Button("Reset Boids"))
	{
		BoidManager::ResetBoids();
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LoadValues()
{
	FILE* pFile;
	pFile = fopen("boids.ini", "r");
	if (pFile == NULL)
	{
		std::cout << "Error opening file boids.ini." << std::endl;
		return;
	}

	char line[256];

	while (fgets(line, sizeof(line), pFile))
	{
		char* equals = strchr(line, '=');
		*equals = '\0';
		char* key = line;
		char* value = equals + 1;

		if (strcmp(key, "MoveSpeed") == 0)
		{
			BoidManager::MoveSpeed = atof(value);
		}
		else if (strcmp(key, "CohesionFactor") == 0)
		{
			BoidManager::CohesionFactor = atof(value);
		}
		else if (strcmp(key, "CohesionDistance") == 0)
		{
			BoidManager::CohesionDistance = atof(value);
		}
		else if (strcmp(key, "SeparationFactor") == 0)
		{
			BoidManager::SeparationFactor = atof(value);
		}
		else if (strcmp(key, "SeparationDistance") == 0)
		{
			BoidManager::SeparationDistance = atof(value);
		}
		else if (strcmp(key, "AlignmentFactor") == 0)
		{
			BoidManager::AlignmentFactor = atof(value);
		}
		else if (strcmp(key, "AlignmentDistance") == 0)
		{
			BoidManager::AlignmentDistance = atof(value);
		}
	}

	BoidManager::ResetBoids();
}

void SaveValues()
{
	FILE* pFile;
	pFile = fopen("boids.ini", "w");
	if (pFile == NULL)
	{	
		std::cout << "Error creating file boids.ini." << std::endl;
		return;
	}

	fprintf(pFile, "MoveSpeed=%f\n", BoidManager::MoveSpeed);
	fprintf(pFile, "CohesionFactor=%f\n", BoidManager::CohesionFactor);
	fprintf(pFile, "CohesionDistance=%f\n", BoidManager::CohesionDistance);
	fprintf(pFile, "SeparationFactor=%f\n", BoidManager::SeparationFactor);
	fprintf(pFile, "SeparationDistance=%f\n", BoidManager::SeparationDistance);
	fprintf(pFile, "AlignmentFactor=%f\n", BoidManager::AlignmentFactor);
	fprintf(pFile, "AlignmentDistance=%f", BoidManager::AlignmentDistance);
	fclose(pFile);
}