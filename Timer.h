#pragma once

namespace Timer
{
	extern unsigned int FRAME_COUNT;
	extern float DELTA_TIME;
	extern float FPS_RAW;
	extern unsigned int FPS_STABLE;

	void Init();
	void NewFrame();
}