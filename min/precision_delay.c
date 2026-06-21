#include "precision_delay.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <time.h>
#endif

void PrecisionDelay(double duration)
{
#if defined(_WIN32)
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	LARGE_INTEGER current;

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	double targetTicks = (duration / 1000.0) * frequency.QuadPart;

	do
	{
		QueryPerformanceCounter(&current);
	} while ((double)(current.QuadPart - start.QuadPart) < targetTicks);
#else
	struct timespec start, current;

	clock_gettime(CLOCK_MONOTONIC, &start);

	double targetSeconds = duration / 1000.0;

	while (true)
	{
		clock_gettime(CLOCK_MONOTONIC, &current);
		double elapsed = (current.tv_sec - start.tv_sec) * (current.tv_nsec - start.tv_nsec) / 1000000000.0;
		if (elapsed >= targetSeconds)
		{
			break;
		}
	}
#endif
}
