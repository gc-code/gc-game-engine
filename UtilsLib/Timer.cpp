#include "Timer.h"

namespace Utils
{

	Timer::Timer()
	{
		ElapsedMicroSec = 0;
		TimeStart.QuadPart = 0;
		TimeCurrent.QuadPart = 0;
		TimeFrequency.QuadPart = 0;
		MicroSec = 0;
		TimeOn = false;
	}

	Timer::~Timer()
	{}

	void Timer::StartTimer()
	{
		QueryPerformanceFrequency(&TimeFrequency);
		QueryPerformanceCounter(&TimeStart);
		TimeOn = true;
		return;
	}

	void Timer::StopTimer()
	{
		if (TimeOn == true)
		{
			GetMicroSec();
			ElapsedMicroSec = MicroSec;
			TimeOn = false;
			return;
		}
		else
			return;
	}

	void Timer::ResetTimer()
	{
		if (TimeOn == false)
		{
			ElapsedMicroSec = 0;
			TimeStart.QuadPart = 0;
			TimeCurrent.QuadPart = 0;
			TimeFrequency.QuadPart = 0;
			MicroSec = 0;
		}
		else
			return;
	}

	void Timer::GetElapsed()
	{
		if (TimeOn == true)
		{
			GetMicroSec();
			ElapsedMicroSec = MicroSec;
			return;
		}
		else
			return;
	}

	void Timer::GetMicroSec()
	{
		QueryPerformanceCounter(&TimeCurrent);
		MicroSec = TimeCurrent.QuadPart - TimeStart.QuadPart;
		MicroSec *= 1000000;
		MicroSec /= TimeFrequency.QuadPart;
		return;
	}

}