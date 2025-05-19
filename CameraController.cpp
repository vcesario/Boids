#include "CameraController.h"
#include "Input.h"
#include "Ui.h"
#include "Timer.h"

CameraController::CameraController(Camera* camera)
{
	MasterCam = camera;

	MovementSpeed = 15.0f;
	RotateSensitivity = 0.3f;
	PanSensitivity = 0.05f;

	m_FocalPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	m_IsDraggingScene = false;
	m_IsResettingFocalPoint = false;
	m_SmoothXDiff = m_SmoothYDiff = 0;
	m_RotateAttenuation = 0.92f;
}

void CameraController::Update()
{
	if (Input::IsActionDown(Input::LEFTDRAG) && !Ui::IsHovered())
	{
		m_IsDraggingScene = true;
	}
	if (Input::IsActionUp(Input::LEFTDRAG))
	{
		m_IsDraggingScene = false;
	}

	if (m_IsDraggingScene)
	{
		RotateAroundFocalPoint(Input::XDiff, -Input::YDiff);
		SetSmoothDiff(Input::XDiff, Input::YDiff);
	}
	else
	{
		SmoothRotate();
	}

	if (Input::IsActionActive(Input::RIGHTDRAG))
	{
		m_IsResettingFocalPoint = false;
		Pan(Input::XDiff, -Input::YDiff);
	}

	if (Input::IsActionDown(Input::ZOOM))
	{
		Zoom(Input::ScrollYDiff);
	}

	if (Input::IsActionDown(Input::RESETFOCUS))
	{
		m_IsResettingFocalPoint = true;
	}

	if (Input::IsActionActive(Input::FORWARD))
	{
		m_IsResettingFocalPoint = false;
		Move(FORWARD);
	}
	else if (Input::IsActionActive(Input::BACKWARD))
	{
		m_IsResettingFocalPoint = false;
		Move(BACKWARD);
	}
	if (Input::IsActionActive(Input::PANLEFT))
	{
		m_IsResettingFocalPoint = false;
		Move(LEFT);
	}
	else if (Input::IsActionActive(Input::PANRIGHT))
	{
		m_IsResettingFocalPoint = false;
		Move(RIGHT);
	}

	if (m_IsResettingFocalPoint)
	{
		glm::vec3 distance = -m_FocalPoint;
		float length = glm::length(distance);

		float minLength = 0.06f;
		float maxLength = 1.5f;
		float maxRelativeLengthRatio = 0.4f;

		if (length < minLength)
		{
			SetFocalPoint(glm::vec3(0));
			m_IsResettingFocalPoint = false;
		}
		else
		{
			glm::vec3 direction = glm::normalize(distance);

			float speed = glm::min(length * maxRelativeLengthRatio, maxLength);
			speed = glm::max(speed, minLength);

			SetFocalPoint(m_FocalPoint + direction * speed);
		}
	}
}

void CameraController::Pan(float xOffset, float yOffset)
{
	xOffset *= PanSensitivity;
	yOffset *= PanSensitivity;

	glm::vec3 verticalStep = MasterCam->Up * yOffset;
	glm::vec3 horizontalStep = MasterCam->Right * xOffset;

	MasterCam->Position += verticalStep + horizontalStep;
	m_FocalPoint += verticalStep + horizontalStep;
}

void CameraController::RotateAroundFocalPoint(float xOffset, float yOffset)
{
	glm::vec3 cameraPosOrigin = MasterCam->Position - m_FocalPoint;
	float focalDistance = glm::length(cameraPosOrigin);

	float polar = glm::degrees(acos(cameraPosOrigin.y / focalDistance));
	float azimuthal = glm::degrees(atan2(cameraPosOrigin.z, cameraPosOrigin.x));

	polar += yOffset * RotateSensitivity;
	azimuthal += xOffset * RotateSensitivity;

	if (polar > 179.0f)
		polar = 179.0f;
	if (polar < 1.0f)
		polar = 1.0f;

	cameraPosOrigin.x = focalDistance * sin(glm::radians(polar)) * cos(glm::radians(azimuthal));
	cameraPosOrigin.y = focalDistance * cos(glm::radians(polar));
	cameraPosOrigin.z = focalDistance * sin(glm::radians(polar)) * sin(glm::radians(azimuthal));

	MasterCam->Position = cameraPosOrigin + m_FocalPoint;

	MasterCam->Front = m_FocalPoint - MasterCam->Position;
	MasterCam->updateCameraVectors_Front();
}

void CameraController::Move(Camera_Movement direction)
{
	float velocity = MovementSpeed * Timer::DELTA_TIME;
	if (direction == FORWARD)
	{
		glm::vec3 step = MasterCam->Front * velocity;
		MasterCam->Position += step;
		m_FocalPoint += step;
	}
	if (direction == BACKWARD)
	{
		glm::vec3 step = MasterCam->Front * velocity;
		MasterCam->Position -= step;
		m_FocalPoint -= step;
	}
	if (direction == LEFT)
	{
		glm::vec3 step = MasterCam->Right * velocity;
		MasterCam->Position -= step;
		m_FocalPoint -= step;
	}
	if (direction == RIGHT)
	{
		glm::vec3 step = MasterCam->Right * velocity;
		MasterCam->Position += step;
		m_FocalPoint += step;
	}
}

void CameraController::Zoom(float yOffset)
{
	MasterCam->Zoom -= (float)yOffset;
	if (MasterCam->Zoom < 1.0f)
		MasterCam->Zoom = 1.0f;
	if (MasterCam->Zoom > 45.0f)
		MasterCam->Zoom = 45.0f;
}

void CameraController::SetFocalPoint(glm::vec3 newPoint)
{
	glm::vec3 diff = newPoint - m_FocalPoint;

	MasterCam->Position += diff;
	m_FocalPoint = newPoint;
}

void CameraController::SetSmoothDiff(float xOffset, float yOffset)
{
	m_SmoothXDiff = xOffset;
	m_SmoothYDiff = yOffset;
}

void CameraController::SmoothRotate()
{
	if (abs(m_SmoothXDiff) < 0.01f && abs(m_SmoothYDiff) < 0.01f)
	{
		return;
	}

	m_SmoothXDiff *= m_RotateAttenuation;
	m_SmoothYDiff *= m_RotateAttenuation;

	RotateAroundFocalPoint(m_SmoothXDiff, -m_SmoothYDiff);
}