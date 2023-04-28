//
// Platform.API.FrameTimer.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// High precision timer.
//
module;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <Furud.hpp>



export module Furud.Platform.API.FrameTimer;

/** Frame timer */
export namespace Furud
{
	class FrameTimer
	{
	public:
		FrameTimer()
		{
			secondsPerCount = 1000.0 / static_cast<double>(GetPerformanceFrequency());
		}

		FURUD_NODISCARD constexpr double TotalTime() const
		{
			return bStopped
				? ((stopTime - pausedTime) - baseTime) * secondsPerCount
				: ((currTime - pausedTime) - baseTime) * secondsPerCount;
		}

		FURUD_NODISCARD constexpr float GetDeltaTime() const
		{
			return static_cast<float>(deltaTime);
		}

		FURUD_NODISCARD constexpr float GetElapsedTime() const
		{
			return static_cast<float>(timeElapsed);
		}

		FURUD_NODISCARD constexpr unsigned int GetFrameNumber() const
		{
			return framePerElapsed;
		}

		FURUD_INLINE void Reset()
		{
			long long tempCurTime = GetPerformanceCounter();
			baseTime = tempCurTime;
			prevTime = tempCurTime;
			stopTime = 0;
			bStopped = false;
		}

		FURUD_INLINE void Start()
		{
			if (bStopped)
			{
				long long tempCurTime = GetPerformanceCounter();
				pausedTime += (tempCurTime - stopTime);
				prevTime = tempCurTime;
				stopTime = 0;
				bStopped = false;
			}
		}

		FURUD_INLINE void Stop()
		{
			if (!bStopped)
			{
				long long TempCurTime = GetPerformanceCounter();
				stopTime = TempCurTime;
				bStopped = true;
			}
		}

		inline void BeginFrame()
		{
			if (bStopped)
			{
				deltaTime = 0.0;
				return;
			}

			long long tempCurTime = GetPerformanceCounter();
			currTime = tempCurTime;
			deltaTime = (currTime - prevTime) * secondsPerCount;
			deltaTime = deltaTime > 0.0 ? deltaTime : 0.0;
			prevTime = currTime;

			timeElapsed += deltaTime;
			currFrame++;
		}

		inline void EndFrame()
		{
			constexpr double elapsed = 1000.0;
			if (timeElapsed > elapsed)
			{
				timeElapsed = 0;
				framePerElapsed = currFrame - lastFrame;
				lastFrame = currFrame;
			}
		}


	private:
		long long GetPerformanceFrequency() const
		{
			long long frequency;
			::QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
			return frequency;
		}

		long long GetPerformanceCounter() const
		{
			long long counter;
			::QueryPerformanceCounter((LARGE_INTEGER*)&counter);
			return counter;
		}


	private:
		bool bStopped = false;

		double secondsPerCount = 0;
		double deltaTime = -1.0;

		long long baseTime = 0;
		long long pausedTime = 0;
		long long stopTime = 0;
		long long prevTime = 0;
		long long currTime = 0;

		double timeElapsed = 0.0;
		unsigned int lastFrame = 0;
		unsigned int currFrame = 0;
		unsigned int framePerElapsed = 0;
	};
}