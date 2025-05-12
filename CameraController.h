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
	float PanSensitivity = 0.05f;
	float RotateSensitivity = 0.3f;
	glm::vec3 FocalPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	Camera* m_Camera;
	float MovementSpeed = 2.5f;

	CameraController(Camera* camera)
	{
		m_Camera = camera;
	}

	void Update(float deltaTime)
	{
		if (Input::IsActionActive(Input::LEFTDRAG))
		{
			RotateAroundFocalPoint(Input::XDiff, -Input::YDiff);
		}

		if (Input::IsActionActive(Input::RIGHTDRAG))
		{
			Pan(Input::XDiff, -Input::YDiff);
		}

		if (Input::IsActionDown(Input::ZOOM))
		{
			Zoom(Input::ScrollYDiff);
		}

		if (Input::IsActionDown(Input::RESETFOCUS))
		{
			ResetFocalPoint();
		}

		if (Input::IsActionActive(Input::FORWARD))
		{
			Move
			(FORWARD, deltaTime);
		}
		else if (Input::IsActionActive(Input::BACKWARD))
		{
			Move(BACKWARD, deltaTime);
		}
		if (Input::IsActionActive(Input::PANLEFT))
		{
			Move(LEFT, deltaTime);
		}
		else if (Input::IsActionActive(Input::PANRIGHT))
		{
			Move(RIGHT, deltaTime);
		}
	}

private:
	void Pan(float xoffset, float yoffset)
	{
		xoffset *= PanSensitivity;
		yoffset *= PanSensitivity;

		glm::vec3 verticalStep = m_Camera->Up * yoffset;
		glm::vec3 horizontalStep = m_Camera->Right * xoffset;

		m_Camera->Position += verticalStep + horizontalStep;
		FocalPoint += verticalStep + horizontalStep;
	}

	void RotateAroundFocalPoint(float xoffset, float yoffset)
	{
		glm::vec3 cameraPosOrigin = m_Camera->Position - FocalPoint;
		float focalDistance = glm::length(cameraPosOrigin);

		float polar = glm::degrees(acos(cameraPosOrigin.y / focalDistance));
		float azimuthal = glm::degrees(atan2(cameraPosOrigin.z, cameraPosOrigin.x));

		polar += yoffset * RotateSensitivity;
		azimuthal += xoffset * RotateSensitivity;

		if (polar > 179.0f)
			polar = 179.0f;
		if (polar < 1.0f)
			polar = 1.0f;

		cameraPosOrigin.x = focalDistance * sin(glm::radians(polar)) * cos(glm::radians(azimuthal));
		cameraPosOrigin.y = focalDistance * cos(glm::radians(polar));
		cameraPosOrigin.z = focalDistance * sin(glm::radians(polar)) * sin(glm::radians(azimuthal));

		m_Camera->Position = cameraPosOrigin + FocalPoint;

		m_Camera->Front = FocalPoint - m_Camera->Position;
		m_Camera->updateCameraVectors_Front();
	}

	void Move(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
		{
			glm::vec3 step = m_Camera->Front * velocity;
			m_Camera->Position += step;
			FocalPoint += step;
		}
		if (direction == BACKWARD)
		{
			glm::vec3 step = m_Camera->Front * velocity;
			m_Camera->Position -= step;
			FocalPoint -= step;
		}
		if (direction == LEFT)
		{
			glm::vec3 step = m_Camera->Right * velocity;
			m_Camera->Position -= step;
			FocalPoint -= step;
		}
		if (direction == RIGHT)
		{
			glm::vec3 step = m_Camera->Right * velocity;
			m_Camera->Position += step;
			FocalPoint += step;
		}
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void Zoom(float yoffset)
	{
		m_Camera->Zoom -= (float)yoffset;
		if (m_Camera->Zoom < 1.0f)
			m_Camera->Zoom = 1.0f;
		if (m_Camera->Zoom > 45.0f)
			m_Camera->Zoom = 45.0f;
	}

	void ResetFocalPoint()
	{
		m_Camera->Position -= FocalPoint;
		FocalPoint = glm::vec3(0);
	}
};