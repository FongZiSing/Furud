//
// Semaphore.hpp
//
//       Copyright (c) Furud IPC Library. All rights reserved.
//       @Author FongZiSing
//
// Implemention of semaphore.
//
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

#include "Name.hpp"
#include "Security.hpp"
#include "Logger.hpp"



namespace Furud::IPC
{
	class ISemaphore final
	{
	private:
		/** Native name. */
		IName name { L"__FURUD_IPC_SEM__" };

		/** Native security. */
		ISecurity security;

		/** Native handle. */
		HANDLE handle = NULL;

		/** Used to ensure realesed by only one process. */
		bool bReadOnly = false;


	public:
		// the maximum number of milliseconds before the wait times out.
		enum : UINT32 { Timeout = UINT32(-1) /* 4294967295u */ };


	public:
		enum class EFlag : UINT32
		{
			Create,
			Open,
		};


	private:
		/** Noncopyable. */
		ISemaphore(const ISemaphore&) = delete;

		/** Noncopyable. */
		ISemaphore& operator = (const ISemaphore&) = delete;

		/** Disable new. */
		void* operator new (size_t) = delete;


	public:
		ISemaphore() noexcept {}

		~ISemaphore() noexcept
		{
			if (handle && !bReadOnly)
			{
				::CloseHandle(handle);
				handle = NULL;
			}
		}

		bool Open(wchar_t const* inSemaphoreName, UINT32 inSemaphoreCount, EFlag SemaphoreFlag) noexcept
		{
			name.Construct(inSemaphoreName);

			if (SemaphoreFlag == EFlag::Open)
			{
				// Opens an existing named semaphore object.
				// https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-opensemaphorew
				handle = OpenSemaphoreW
					( EVENT_ALL_ACCESS
					, false
					, name.data
				);
				bReadOnly = true;
			}
			else
			{
				// Creates or opens a named or unnamed semaphore object.
				// see https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createsemaphorew
				handle = CreateSemaphoreW
					( security.data
					, static_cast<LONG>(inSemaphoreCount)
					, static_cast<LONG>(inSemaphoreCount)
					, name.data
					);
				bReadOnly = false;

				if (handle && GetLastError() == ERROR_ALREADY_EXISTS)
				{
					Logger::Error("Failed to create semaphore, semaphore has existed.`");
					::CloseHandle(handle);
					handle = NULL;
				}
			}

			return !!handle;
		}

		bool Wait(UINT32 timeout = 0) noexcept
		{
			if (handle)
			{
				DWORD ret = ::WaitForSingleObject(handle, static_cast<DWORD>(timeout));

				switch (ret)
				{
				case WAIT_OBJECT_0:
					return true;

				case WAIT_TIMEOUT:
					return false;

				case WAIT_ABANDONED:
				default:
					Logger::Error("Failed to call `WaitForSingleObject` with error code `%lu`, return code `0x%08X`", ::GetLastError(), ret);
					break;
				}
			}
			return false;
		}

		bool Post(UINT32 count = 1) noexcept
		{
			if (handle)
			{
				// Release the semaphore.
				// see https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-releasesemaphore
				if (!::ReleaseSemaphore(handle, static_cast<LONG>(count), nullptr))
				{
					Logger::Error("Failed to call `ReleaseSemaphore` with error code `%lu`", ::GetLastError());
					return false;
				}
				return true;
			}
			return false;
		}
	};
}