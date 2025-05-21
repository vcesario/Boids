#pragma once

namespace Timer
{
	extern unsigned long long FrameCount;
	extern float DeltaTime;
	extern float FpsRaw;
	extern unsigned int FpsStable;

	void Init();
	void NewFrame();
}