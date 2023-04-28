//
// Platform.Thread.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Thread interface.
//
module;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <Furud.hpp>
#include <process.h>
#include <stdint.h>



export module Furud.Platform.Thread;

/** Thread interface. */
namespace Furud::Internal
{
	/**
	 * @brief    Interface for threads.
	 *           This interface specifies the function used to manager thread's life cycle.
	 * @details  线程接口。
	 */
	class FURUD_INTERFACE IThread
	{
	public:
		enum class Priority : uint8_t
		{
			Low,
			Normal,
			High,
			Critical,
			Default = Normal,
		};


		enum class InitFlag :uint8_t
		{
			CreateRunnable,
			CreateSuspended,
		};


		struct Option
		{
			const char* threadName = nullptr;
			Priority threadPriority = Priority::Default;
			InitFlag threadInitFlag = InitFlag::CreateRunnable;
		};


	public:
		constexpr IThread() = default;

		/** Noncopyable. */
		IThread(const IThread&) = delete;

		/** Noncopyable. */
		IThread& operator = (const IThread&) = delete;


	public:
		/**
		 * @brief    Initializes Thread with the specified option.
		 * @details  初始化线程。
		 */
		virtual bool Init(Option = {}) = 0;


		/**
		 * @brief    Tells the thread to either pause execution or resume.
		 * @param    bPause  -  Pause the thread if true, else resume.
		 *
		 * @details  挂起/激活线程。
		 */
		virtual void Suspend(bool bPause) = 0;


		/**
		 * @brief    Exits the thread.
		 * @param    bWait  -  Whether to wait infinitely until this thread exits.
		 * @details  结束线程。
		 */
		virtual void Kill(bool bWait) = 0;


		/**
		 * @brief    Waits for the thread to finish work.
		 * @details  等待线程。
		 */
		virtual void WaitForCompletion() = 0;


		/**
		 * @brief    Sets the priority value for this thread.
		 * @details  设置线程优先级。
		 */
		virtual void SetPriority(Priority priority) = 0;


	protected:
		/**
		 * @brief    The real thread entry point.
		 * @details  线程中执行的函数。
		 */
		virtual void Run() = 0;
	};
}



/** Thread utility. */
namespace Furud::Internal
{
	/**
	 * @brief    Setting a thread name in native code.
	 * @see      see http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
	 * @details  设置当前所在线程的名字。
	 */
	void SetThreadName(const char* threadName)
	{
		const uint32_t MS_VC_EXCEPTION = 0x406D1388;
		struct THREADNAME_INFO
		{
			DWORD dwType;     // Must be 0x1000.  
			LPCSTR szName;    // Pointer to name (in user addr space).  
			DWORD dwThreadID; // Thread ID (-1=caller thread).  
			DWORD dwFlags;    // Reserved for future use, must be zero.  
		};

		THREADNAME_INFO ThreadNameInfo;
		ThreadNameInfo.dwType = 0x1000;
		ThreadNameInfo.szName = threadName;
		ThreadNameInfo.dwThreadID = ::GetCurrentThreadId();
		ThreadNameInfo.dwFlags = 0;

#pragma warning(push)  
#pragma warning(disable: 6320 6322)  
		__try
		{
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(ThreadNameInfo) / sizeof(ULONG_PTR), (ULONG_PTR*)&ThreadNameInfo);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			// TODO
		}
#pragma warning(pop)
	}


	/**
	 * @brief    Assigns a description to a thread.
	 * @see      https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreaddescription
	 * @details  设置当前所在线程的描述符。
	 */
	void SetThreadDescription(const wchar_t* threadDescription)
	{
		typedef HRESULT(WINAPI* SetThreadDescriptionFnPtr)(HANDLE hThread, PCWSTR lpThreadDescription);

#pragma warning( push )
#pragma warning( disable: 4191 ) // unsafe conversion from 'type of expression' to 'type required'
		static SetThreadDescriptionFnPtr RealSetThreadDescription = (SetThreadDescriptionFnPtr)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "SetThreadDescription");
#pragma warning( pop )

		if (RealSetThreadDescription)
		{
			RealSetThreadDescription(::GetCurrentThread(), threadDescription);
		}
	}
}



/** Thread implementation. */
export namespace Furud
{
	class Thread : public Internal::IThread
	{
	private:
		/** Detail information of the thread. */
		struct Detail
		{
			static constexpr int32_t length  = 32;
			wchar_t description[length]    = {};
			char name[length]              = {};
			uint32_t id                      = 0;
		}
		details;
		
		/** The handle to this thread. */
		void* handle;
		
		/** The priority value to this thread. */
		IThread::Priority priority;


	public:
		Thread()
			: details()
			, handle(nullptr)
			, priority(Priority::Default)
		{}

		virtual ~Thread()
		{
			Kill(true);
		}


		/**
		 * @brief    Initializes Thread with the specified option.
		 * @details  初始化线程。
		 */
		virtual bool Init(IThread::Option option) final
		{
			// Create thread once.
			if (handle)
			{
				return false;
			}

			// Launch a new thread.
			// see https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/beginthread-beginthreadex
			handle = (HANDLE)::_beginthreadex
				( nullptr    // the handle can not be inherited by child processes.
				, 0          // stack size.
				, ThreadProc // callee.
				, this       // args.
				, TranslateThreadInitFlag(option.threadInitFlag)
				, &details.id
			);

			if (!handle)
			{
				details.id = 0;
				return false;
			}

			// Initialize thread priority.
			SetPriority(option.threadPriority);

			// Initialize thread name.
			SetThreadName(option.threadName);

			return true;
		}


		/**
		 * @brief    Tells the thread to either pause execution or resume.
		 * @param    bPause  -  Pause the thread if true, else resume.
		 *
		 * @details  挂起/激活线程。
		 */
		virtual void Suspend(bool bPause) final
		{
			if (handle)
			{
				if (bPause)
				{
					// Suspends current thread.
					// see https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-suspendthread
					::SuspendThread(handle);
				}
				else
				{
					// Decrements current thread's suspend count.
					// When the suspend count is decremented to zero, the execution of the thread is resumed.
					// see https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-resumethread
					::ResumeThread(handle);
				}
			}
		}


		/**
		 * @brief    Exits the thread.
		 * @param    bWait  -  Whether to wait infinitely until this thread exits.
		 * @details  结束线程。
		 */
		virtual void Kill(bool bWait) final
		{
			if (handle)
			{
				if (bWait)
				{
					// It's not safe to just kill the thread with `TerminateThread()` as it could have a
					// mutex lock that's shared with a thread's continuing to run, which would cause that
					// other thread to dead-lock.
					::WaitForSingleObject(handle, INFINITE);
				}

				::CloseHandle(handle);
				handle = nullptr;
				details.id = 0;
			}
		}


		/**
		 * @brief    Waits for the thread to finish work.
		 * @details  等待线程。
		 */
		virtual void WaitForCompletion() final
		{
			if (handle)
			{
				// Waits until this thread is in the signaled state or the time-out interval elapses.
				// see https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
				::WaitForSingleObject(handle, INFINITE);
			}
		}


		/**
		 * @brief    Sets the priority value for this thread.
		 * @details  设置线程优先级。
		 */
		virtual void SetPriority(Priority inPriority) final
		{
			if (handle)
			{
				priority = inPriority;

				// Sets the priority value for the specified thread.
				// see https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreadpriority
				::SetThreadPriority(handle, TranslateThreadPriority(inPriority));
			}
		}


	private:
		/**
		 * @brief    Reset the thread name.
		 * @details  设置线程名字。
		 */
		void SetThreadName(const char* inThreadName)
		{
			const int32_t dstlen = details.length - 1;
			const int32_t srclen = (int32_t)__builtin_strlen(inThreadName);
			int32_t len = (dstlen < srclen ? dstlen : srclen);

			// Copy origin string.
			details.name[len] = 0;
			::memcpy(details.name, inThreadName, len);

			// Convert to widechar string.
			details.description[len] = 0;
			MultiByteToWideChar(CP_ACP, 0, details.name, len, details.description, len);
		}


		/**
		 * @brief    The real thread entry point.
		 * @details  线程中执行的函数。
		 */
		static unsigned __stdcall ThreadProc(void* args)
		{
			Thread* this_thread = reinterpret_cast<Thread*>(args);
			if (this_thread)
			{
				Internal::SetThreadDescription(this_thread->details.description);
				Internal::SetThreadName(this_thread->details.name);

				this_thread->Run();
			}

			// Given to terminate.
			// see https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/endthread-endthreadex
			_endthreadex(0);

			return 0;
		}


	protected:
		/**
		 * @brief    The real thread entry point.
		 * @details  线程中执行的函数。
		 */
		virtual void Run() override {}


	private:
		static int TranslateThreadPriority(IThread::Priority priority)
		{
			// Translates the enumeration to thread's priority value.
			// see https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreadpriority
			switch (priority)
			{
			case IThread::Priority::Low:      return THREAD_PRIORITY_LOWEST;
			case IThread::Priority::Normal:   return THREAD_PRIORITY_NORMAL;
			case IThread::Priority::High:     return THREAD_PRIORITY_HIGHEST;
			case IThread::Priority::Critical: return THREAD_PRIORITY_TIME_CRITICAL;

			default: return THREAD_PRIORITY_NORMAL;
			}
		}


		static unsigned TranslateThreadInitFlag(IThread::InitFlag initFlag)
		{
			// Translates the enumeration to thread's creation flag.
			// see https://learn.microsoft.com/en-us/windows/win32/procthread/process-creation-flags#flags
			return (initFlag == InitFlag::CreateSuspended) ? CREATE_SUSPENDED : 0;
		}
	};
}