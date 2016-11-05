#ifndef CGAMETIMER_H
#define CGAMETIMER_H

#include <windows.h>

class CGameTimer
{
private:
	double mSecondsPerCount;

	// The difference in time since the last time the timer was called.
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
public:
	// Constructor.
	CGameTimer();

	// Total time since timer was started measured in seconds.
	float TotalTime()const;

	// Difference in time between this time and the last time the timer was called.
	float DeltaTime()const;

	// Reset before the message loop.
	void Reset();

	// Start the timer when unpaused.
	void Start();

	// Stop the timer when paused.
	void Stop();

	// The tick function should be called every frame.
	void Tick();

	// Destructor.
	~CGameTimer();
};

#endif