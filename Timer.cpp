#include <GLFW/glfw3.h>

#include "Timer.h"

namespace Timer
{
	const unsigned int FPS_ARRAY_SIZE = 100;

	unsigned int FRAME_COUNT;
	float DELTA_TIME;
	float FPS_RAW;
	float FPS_STABLE;

	float previousTime;
	float currentTime;
	float fpsArray[FPS_ARRAY_SIZE];

	void Init()
	{
		previousTime = 0;
		currentTime = 0;

		FRAME_COUNT = -1;
		DELTA_TIME = 0;
		FPS_RAW = 0;
	}

	void NewFrame()
	{
		FRAME_COUNT++;
		previousTime = currentTime;
		currentTime = glfwGetTime();
		DELTA_TIME = currentTime - previousTime;
		FPS_RAW = 1.0f / DELTA_TIME;


		int fpsArrayIndex = FRAME_COUNT % FPS_ARRAY_SIZE;
		fpsArray[fpsArrayIndex] = FPS_RAW;

		float avgArray = 0;
		for (size_t i = 0; i < FPS_ARRAY_SIZE && i < FRAME_COUNT; i++)
		{
			avgArray += fpsArray[i];
		}
		if (FRAME_COUNT < FPS_ARRAY_SIZE)
		{
			avgArray /= FRAME_COUNT;
		}
		else
		{
			avgArray /= FPS_ARRAY_SIZE;
		}

		FPS_STABLE = avgArray;
	}
}