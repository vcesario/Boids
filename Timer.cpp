#include <GLFW/glfw3.h>

#include "Timer.h"

namespace Timer
{
	const unsigned int FPS_ARRAY_SIZE = 1000;

	unsigned int FRAME_COUNT;
	float DELTA_TIME;
	float FPS_RAW;
	unsigned int FPS_STABLE;

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
		unsigned int maxSize = FRAME_COUNT > FPS_ARRAY_SIZE ? FPS_ARRAY_SIZE : FRAME_COUNT;
		for (size_t i = 0; i < maxSize; i++)
		{
			avgArray += fpsArray[i];
		}
		avgArray /= maxSize;

		FPS_STABLE = avgArray;

		// TODO: this is improvable, by keeping a global ArraySum instead of running a for loop every frame.
	}
}