#ifndef _TIMER_H
#define _TIMER_H

#include <Windows.h>

namespace Utils
{

	class Timer
	{
	public:
		Timer();

		~Timer();

		void StartTimer();

		void StopTimer();

		void ResetTimer();

		void GetElapsed();

		LONGLONG ElapsedMicroSec;

	private:
		void GetMicroSec();

		LARGE_INTEGER TimeStart;

		LARGE_INTEGER TimeCurrent;

		LARGE_INTEGER TimeFrequency;

		LONGLONG MicroSec;

		bool TimeOn;
	};

}

#endif