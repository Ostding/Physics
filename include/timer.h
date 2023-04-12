#ifndef __PHYSIC_TIMER_H__
#define __PHYSIC_TIMER_H__
#include <iostream>
#include <chrono>
#include "my_printf.h"
using namespace std;
using namespace chrono;

namespace physics
{

	class Timer
	{
	public:
		Timer()
		{
			Timer::start();
		};

		~Timer()
		{
			Timer::stop();
		};

	public:
		static void start()
		{
			Timer::_start = system_clock::now();
		};

		static void stop(const char *pszCap = "")
		{
			auto end = system_clock::now();
			auto duration = duration_cast<microseconds>(end - Timer::_start);
			double t = double(duration.count()) * microseconds::period::num / microseconds::period::den;
			myPrintf("Time duration %s:%.8fs \n", pszCap, t);
		};

	public:
		static system_clock::time_point _start;
	};

}
#endif