//
// Mutex.hpp
//
//       Copyright (c) Furud IPC Library. All rights reserved.
//       @Author FongZiSing
//
// Implemention of mutex.
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
	class IMutex final
	{
	private:
		/** Native name. */
		IName name { L"__FURUD_IPC_MTX__" };

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
		IMutex(const IMutex&) = delete;

		/** Noncopyable. */
		IMutex& operator = (const IMutex&) = delete;

		/** Disable new. */
		void* operator new (size_t) = delete;


	public:
		IMutex() noexcept {}

		~IMutex() noexcept
		{
			if (handle && !bReadOnly)
			{
				::CloseHandle(handle);
				handle = NULL;
			}
		}

		bool Open(wchar_t const* inMutexName, UINT32 inMutexCount, EFlag MutexFlag) noexcept
		{
			name.Construct(inMutexName);

			if (MutexFlag == EFlag::Open)
			{
				// Opens an existing named mutex object.
				// https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-openmutexw
				handle = OpenMutexW
					( EVENT_ALL_ACCESS
					, false
					, name.data
				);
				bReadOnly = true;
			}
			else
			{
				// Creates or opens a named mutex object.
				// see https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createmutexw
				handle = CreateMutexW
					( security.data
					, false
					, name.data
					);
				bReadOnly = false;

				if (handle && ::GetLastError() == ERROR_ALREADY_EXISTS)
				{
					Logger::Error("Failed to create mutex, mutex has existed.`");
					::CloseHandle(handle);
					handle = NULL;
				}
			}

			return !!handle;
		}

		bool Lock(UINT32 timeout) noexcept
		{
			if (handle)
			{
				while (true)
				{
					DWORD ret = ::WaitForSingleObject(handle, static_cast<DWORD>(timeout));
					switch (ret)
					{
					case WAIT_OBJECT_0:
						return true;

					case WAIT_TIMEOUT:
						return false;

					case WAIT_ABANDONED:
						Logger::Log("Failed to call `WaitForSingleObject` with error code `%lu`, return code `WAIT_ABANDONED`", ::GetLastError());
						if (!Unlock())
						{
							return false;
						}
						break; // loop
					default:
						Logger::Error("Failed to call `WaitForSingleObject` with error code `%lu`, return code `0x%08X`", ::GetLastError(), ret);
						return false;
					}
				}
			}
		}

		bool TryLock() noexcept
		{
			DWORD ret = ::WaitForSingleObject(handle, 0);
			switch (ret)
			{
			case WAIT_OBJECT_0:
				return true;

			case WAIT_TIMEOUT:
				return false;

			case WAIT_ABANDONED:
				Unlock();
				[[fallthrough]];

			default:
				Logger::Error("Failed to call `WaitForSingleObject` with error code `%lu`, return code `0x%08X`", ::GetLastError(), ret);
				return false;
			}
		}

		bool Unlock() noexcept
		{
			if (handle)
			{

				// Release the mutex.
				// see https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-releasemutex
				if (!::ReleaseMutex(handle))
				{
					Logger::Error("Failed to call `ReleaseMutex` with error code `%lu`", ::GetLastError());
					return false;
				}
				return true;
			}
			return false;
		}
	};
}