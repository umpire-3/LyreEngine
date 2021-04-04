#include "LyrePch.h"
#include "Core/Platform.h"


float Lyre::IPlatform::GetTime()
{
	LARGE_INTEGER CurrentTime;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&CurrentTime);

	CurrentTime.QuadPart *= 1000000;
	CurrentTime.QuadPart /= Frequency.QuadPart;

	return CurrentTime.QuadPart / 1000000.f;
}


float Lyre::IPlatform::GetDeltaTimeSinceLastCall()
{
	static LARGE_INTEGER LastCallTime = LARGE_INTEGER{ 0 };

	LARGE_INTEGER CurrentTime;
	LARGE_INTEGER DeltaTime;
	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&CurrentTime);

	DeltaTime.QuadPart = CurrentTime.QuadPart - LastCallTime.QuadPart;
	LastCallTime.QuadPart = CurrentTime.QuadPart;

	DeltaTime.QuadPart *= 1000000;
	DeltaTime.QuadPart /= Frequency.QuadPart;

	return DeltaTime.QuadPart / 1000000.f;
}
