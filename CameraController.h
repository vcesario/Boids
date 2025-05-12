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
			Move(FORWARD, deltaTime);
		}
		else if (Input::IsActionActive(Input::BACKWARD))
		{
			m_IsResettingFocalPoint = false;
			Move(BACKWARD, deltaTime);
		}
		if (Input::IsActionActive(Input::PANLEFT))
		{
			m_IsResettingFocalPoint = false;
			Move(LEFT, deltaTime);
		}
		else if (Input::IsActionActive(Input::PANRIGHT))
		{
			m_IsResettingFocalPoint = false;
			Move(RIGHT, deltaTime);
		}

		if (m_IsResettingFocalPoint)
		{
			glm::vec3 distance = -FocalPoint;
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

				SetFocalPoint(FocalPoint + direction * speed);
			}
		}
	}

private:
	const float ROTATE_ATTENUATION = 0.92f;
	Camera* m_Camera;
	float SmoothXDiff, SmoothYDiff;
	bool m_IsResettingFocalPoint = false;

	void Pan(float xOffset, float yOffset)
	{
		xOffset *= PanSensitivity;
		yOffset *= PanSensitivity;

		glm::vec3 verticalStep = m_Camera->Up * yOffset;
		glm::vec3 horizontalStep = m_Camera->Right * xOffset;

		m_Camera->Position += verticalStep + horizontalStep;
		FocalPoint += verticalStep + horizontalStep;
	}

	void RotateAroundFocalPoint(float xOffset, float yOffset)
	{
		glm::vec3 cameraPosOrigin = m_Camera->Position - FocalPoint;
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

	void Zoom(float yOffset)
	{
		m_Camera->Zoom -= (float)yOffset;
		if (m_Camera->Zoom < 1.0f)
			m_Camera->Zoom = 1.0f;
		if (m_Camera->Zoom > 45.0f)
			m_Camera->Zoom = 45.0f;
	}

	void SetFocalPoint(glm::vec3 newPoint)
	{
		glm::vec3 diff = newPoint - FocalPoint;

		m_Camera->Position += diff;
		FocalPoint = newPoint;
	}

	void SetSmoothDiff(float xOffset, float yOffset)
	{
		SmoothXDiff = xOffset;
		SmoothYDiff = yOffset;
	}

	void SmoothRotate()
	{
		if (abs(SmoothXDiff) < 0.01f && abs(SmoothYDiff) < 0.01f)
		{
			return;
		}

		SmoothXDiff *= ROTATE_ATTENUATION;
		SmoothYDiff *= ROTATE_ATTENUATION;

		RotateAroundFocalPoint(SmoothXDiff, -SmoothYDiff);
	}
};