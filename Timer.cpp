#include <GLFW/glfw3.h>

#include "Timer.h"

namespace Timer
{
	const unsigned int FPS_SUM_SPAN_SIZE = 1000;

	unsigned long long FrameCount;
	float DeltaTime;
	float FpsRaw;
	unsigned int FpsStable;

	float m_PreviousTime;
	float m_CurrentTime;
	float m_FpsSum;
	float m_PreviousFpsRaw;

	void Init()
	{
		m_PreviousTime = 0;
		m_CurrentTime = 0;

		FrameCount = -1;
		DeltaTime = 0;
		FpsRaw = 0;
		FpsStable = 0;
	}

	void NewFrame()
	{
		FrameCount++;
		m_PreviousTime = m_CurrentTime;
		m_CurrentTime = glfwGetTime();
		DeltaTime = m_CurrentTime - m_PreviousTime;
		m_PreviousFpsRaw = FpsRaw;
		FpsRaw = 1.0f / DeltaTime;

		float arraySize = FrameCount;
		if (FrameCount > FPS_SUM_SPAN_SIZE)
		{
			m_FpsSum -= m_PreviousFpsRaw;
			arraySize = FPS_SUM_SPAN_SIZE;
		}
		m_FpsSum += FpsRaw;
		FpsStable = m_FpsSum / arraySize;
	}
}