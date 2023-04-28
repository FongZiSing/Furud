//
// Platform.Thread.TinyTask.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Warpper Of Windows Concurrency Runtime.
//
module;

#include <concrt.h>



export module Furud.Platform.Thread.TinyTask;

namespace Furud::Internal
{
	/**
	 * @brief  A runnable lightweiget async task base on window concurrency runtime library.
	 * @see    https://docs.microsoft.com/en-us/cpp/parallel/concrt/reference/currentscheduler-class?view=msvc-170#scheduletask
	 * @see    https://docs.microsoft.com/en-us/cpp/parallel/concrt/walkthrough-adapting-existing-code-to-use-lightweight-tasks?view=msvc-170
	 */
	class TinyTask
	{
		Concurrency::event signal;


	public:
		void Start(Concurrency::TaskProc task, void* data)
		{
			signal.reset();
			Concurrency::CurrentScheduler::ScheduleTask(task, data);
		}

		void Finish()
		{
			signal.set();
		}

		size_t Wait(uint32_t timeout)
		{
			return signal.wait(timeout);
		}
	};
}



export namespace Furud
{
	/**
	 * @brief    Asynchronous task.
	 * @details  异步任务。
	 */
	class TinyTask
	{
		Internal::TinyTask task;


	public:
		// the maximum number of milliseconds before the wait times out.
		enum : uint32_t { Timeout = uint32_t(-1) /* 4294967295u */ };


	private:
		static void __cdecl DoWorkInternal(void* instance)
		{
			TinyTask* pThis = static_cast<TinyTask*>(instance);
			if (pThis->Init())
			{
				pThis->DoWork();
			}
			
			pThis->task.Finish();
		}


	protected:
		/**
		 * @brief    Initializes the task.
		 * @details  初始化异步任务。
		 */
		virtual bool Init() { return true; }

		/**
		 * @brief    Overrides this function to do you job.
		 * @details  重载实现异步任务。
		 */
		virtual void DoWork() = 0;


	public:
		/**
		 * @brief    Start the async task, please make sure the task is not running now.
		 * @details  开始执行异步任务。
		 */
		inline void Start()
		{
			task.Start(DoWorkInternal, this);
		}

		/**
		 * @brief   Waits for the task to become finished.
		 * @return  If the wait was satisfied, the value `0` is returned;
		 *          otherwise, the value `4294967295u` to indicate that the wait timed out.
		 * @details  等待异步任务完成。
		 */
		inline uint64_t Wait(uint32_t timeout = Timeout)
		{
			return task.Wait(timeout);
		}
	};
}