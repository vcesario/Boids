#pragma once

#include "Camera.h"

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class CameraController
{
public:
	float MovementSpeed;
	float RotateSensitivity;
	float PanSensitivity;
	Camera* MasterCam;

	CameraController(Camera* camera);
	void Update();

private:
	glm::vec3 m_FocalPoint;
	float m_RotateAttenuation;
	float m_SmoothXDiff, m_SmoothYDiff;
	bool m_IsResettingFocalPoint;
	bool m_IsDraggingScene;

	void Pan(float xOffset, float yOffset);
	void RotateAroundFocalPoint(float xOffset, float yOffset);
	void Move(Camera_Movement direction);
	void Zoom(float yOffset);
	void SetFocalPoint(glm::vec3 newPoint);
	void SetSmoothDiff(float xOffset, float yOffset);
	void SmoothRotate();
};