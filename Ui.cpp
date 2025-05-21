#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Ui.h"
#include "Useful.h"
#include "BoidManager.h"
#include "Timer.h"

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

	static ImGuiWindowFlags fpsFlags = ImGuiWindowFlags_NoInputs
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoBackground
		;
	
	ImGui::Begin("FPS", NULL, fpsFlags);
	{
		static ImVec2 fpsWindowSize(100, 50);
		static ImVec2 fpsWindowPos(5, 5);
		ImGui::SetWindowSize(fpsWindowSize, ImGuiCond_Once);
		ImGui::SetWindowPos(fpsWindowPos, ImGuiCond_Once);

		ImGui::TextDisabled("%d", Timer::FpsStable);
	}
	ImGui::End();

	//ImGui::ShowDemoWindow();

	ImGui::Begin("Inspector");
	{
		ImGui::SetWindowSize(m_InspectorWindowSize, ImGuiCond_Once);
		ImGui::SetWindowPos(m_InspectorWindowPos, ImGuiCond_Once);

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
			ImGui::SliderFloat("MoveSpeed", &BoidManager::MoveSpeed, 0, 10);
			ImGui::SliderFloat("Angle of Sight", &BoidManager::AngleOfSight, -1, 1);
			ImGui::SliderFloat("Sight Range", &BoidManager::SightRange, 0, 20);
			ImGui::DragFloat("Cohesion Factor", &BoidManager::CohesionFactor, 0.005f, 0, 10);
			ImGui::DragFloat("Separation Factor", &BoidManager::SeparationFactor, 0.005f, 0, 10);
			ImGui::DragFloat("Alignment Factor", &BoidManager::AlignmentFactor, 0.005f, 0, 10);
			ImGui::SliderFloat("Wall Avoidance Factor", &BoidManager::WallAvoidanceFactor, 0, 50);
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
		else if (strcmp(key, "AngleOfSight") == 0)
		{
			BoidManager::AngleOfSight = atof(value);
		}
		else if (strcmp(key, "SightRange") == 0)
		{
			BoidManager::SightRange = atof(value);
		}
		else if (strcmp(key, "CohesionFactor") == 0)
		{
			BoidManager::CohesionFactor = atof(value);
		}
		else if (strcmp(key, "SeparationFactor") == 0)
		{
			BoidManager::SeparationFactor = atof(value);
		}
		else if (strcmp(key, "AlignmentFactor") == 0)
		{
			BoidManager::AlignmentFactor = atof(value);
		}
		else if (strcmp(key, "WallAvoidanceFactor") == 0)
		{
			BoidManager::WallAvoidanceFactor = atof(value);
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
	fprintf(pFile, "AngleOfSight=%f\n", BoidManager::AngleOfSight);
	fprintf(pFile, "SightRange=%f\n", BoidManager::SightRange);
	fprintf(pFile, "CohesionFactor=%f\n", BoidManager::CohesionFactor);
	fprintf(pFile, "SeparationFactor=%f\n", BoidManager::SeparationFactor);
	fprintf(pFile, "AlignmentFactor=%f\n", BoidManager::AlignmentFactor);
	fprintf(pFile, "WallAvoidanceFactor=%f\n", BoidManager::WallAvoidanceFactor);
	fclose(pFile);
}