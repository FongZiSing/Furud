//
// Platform.Thread.SpinLock.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Mutex.
//
module;

#include <Furud.hpp>
#include <atomic>



export module Furud.Platform.Thread.SpinLock;

export namespace Furud
{

	/**
	 * @brief    A mutex that doesn't put the thread into a WAIT state but instead repeatedly tries to aquire the lock.
	 * @note     Should be used only for very short locks.
	 * @details  自旋锁。
	 */
	class AtomicSpinLock
	{
		std::atomic<bool> flag { false };


	public:
		AtomicSpinLock() noexcept = default;
		AtomicSpinLock(const AtomicSpinLock&) = delete;
		AtomicSpinLock& operator = (const AtomicSpinLock&) = delete;


	public:
		void Lock()
		{
			while (true)
			{
				if (!flag.exchange(true, std::memory_order_acquire))
				{
					break;
				}

				while (flag.load(std::memory_order_relaxed))
				{
					//std::this_thread::yield();
					_mm_pause();
				}
			}
		}

		FURUD_INLINE bool TryLock() noexcept
		{
			return !flag.exchange(true, std::memory_order_acquire);
		}

		FURUD_INLINE void UnLock() noexcept
		{
			flag.store(false, std::memory_order_release);
		}

		FURUD_INLINE bool IsLock() const noexcept
		{
			return flag.load(std::memory_order_acquire);
		}
	};



	/**
	 * @brief    A RAII structure for AtomicSpinLock.
	 * @note     Should be used only for very short locks.
	 * @details  自旋锁。
	 */
	struct AtomicScopeLock final
	{
		AtomicSpinLock& guard;

		explicit AtomicScopeLock(AtomicSpinLock& mutex) noexcept
			: guard(mutex)
		{
			guard.Lock();
		}

		~AtomicScopeLock()
		{
			guard.UnLock();
		}
	};
}