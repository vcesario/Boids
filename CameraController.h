#include "Camera.h"

class CameraController
{
public:
	float RotateSensitivity = 0.3f;
	glm::vec3 FocalPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	float FocalDistance = 10;
	float Polar = 90;
	float Azimuthal = 90;
	Camera* m_Camera;

	CameraController(Camera* camera)
	{
		m_Camera = camera;
	}

	void Update()
	{
		if (Input::IsDragging)
		{
			RotateAroundFocal(Input::XDiff, -Input::YDiff);
		}
	}

	void RotateAroundFocal(float xoffset, float yoffset)
	{
		xoffset *= RotateSensitivity;
		yoffset *= RotateSensitivity;

		Polar += yoffset;
		Azimuthal += xoffset;

		if (Polar > 179.0f)
			Polar = 179.0f;
		if (Polar < 1.0f)
			Polar = 1.0f;

		glm::vec3 position = m_Camera->Position;

		position.x = FocalDistance * sin(glm::radians(Polar)) * cos(glm::radians(Azimuthal));
		position.y = FocalDistance * cos(glm::radians(Polar));
		position.z = FocalDistance * sin(glm::radians(Polar)) * sin(glm::radians(Azimuthal));

		m_Camera->Position = position;

		m_Camera->Front = -position;
		m_Camera->updateCameraVectors_Front();
	}
};