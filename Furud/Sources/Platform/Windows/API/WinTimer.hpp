//
// WinTimer.hpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// High precision timer.
//
#pragma once

#include <GenericCommon.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>



namespace Furud
{
	class FURUD_API GenericTimer
	{
	public:
		GenericTimer()
		{
			secondsPerCount = 1000.0 / static_cast<double>(GetPerformanceFrequency());
		}

		warn_nodiscard constexpr double TotalTime() const
		{
			return m_bStopped
				? ((stopTime - pausedTime) - baseTime) * secondsPerCount
				: ((currTime - pausedTime) - baseTime) * secondsPerCount;
		}

		warn_nodiscard constexpr float GetDeltaTime() const
		{
			return static_cast<float>(deltaTime);
		}

		warn_nodiscard constexpr float GetElapsedTime() const
		{
			return static_cast<float>(timeElapsed);
		}

		warn_nodiscard constexpr uint32 GetFrameNumber() const
		{
			return framePerElapsed;
		}

		force_inline void Reset()
		{
			int64 tempCurTime = GetPerformanceCounter();
			baseTime = tempCurTime;
			prevTime = tempCurTime;
			stopTime = 0;
			m_bStopped = false;
		}

		force_inline void Start()
		{
			if (m_bStopped)
			{
				int64 tempCurTime = GetPerformanceCounter();
				pausedTime += (tempCurTime - stopTime);
				prevTime = tempCurTime;
				stopTime = 0;
				m_bStopped = false;
			}
		}

		force_inline void Stop()
		{
			if (!m_bStopped)
			{
				int64 TempCurTime = GetPerformanceCounter();
				stopTime = TempCurTime;
				m_bStopped = true;
			}
		}

		inline void BeginFrame()
		{
			if (m_bStopped)
			{
				deltaTime = 0.0;
				return;
			}

			int64 tempCurTime = GetPerformanceCounter();
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
		force_inline int64 GetPerformanceFrequency() const
		{
			int64 frequency;
			::QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
			return frequency;
		}

		force_inline int64 GetPerformanceCounter() const
		{
			int64 counter;
			::QueryPerformanceCounter((LARGE_INTEGER*)&counter);
			return counter;
		}


	private:
		bool m_bStopped = false;

		double secondsPerCount = 0;
		double deltaTime = -1.0;

		int64 baseTime = 0;
		int64 pausedTime = 0;
		int64 stopTime = 0;
		int64 prevTime = 0;
		int64 currTime = 0;

		double timeElapsed = 0.0;
		uint32 lastFrame = 0;
		uint32 currFrame = 0;
		uint32 framePerElapsed = 0;
	};
}