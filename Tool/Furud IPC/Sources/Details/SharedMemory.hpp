//
// SharedMemory.hpp
//
//       Copyright (c) Furud IPC Library. All rights reserved.
//       @Author FongZiSing
//
// Implemention of SharedMemory.
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
	class ISharedMemory final
	{
	private:
		/** Native name. */
		IName name{ L"__FURUD_IPC_SHM__" };
		
		/** Native security. */
		ISecurity security;

		/** Native handle. */
		HANDLE handle = NULL;
		
		void* memory = nullptr;
		
		UINT32 size = 0;


	public:
		enum class EFlag : UINT32
		{
			Create,
			Open,
		};


	private:
		/** Noncopyable. */
		ISharedMemory(const ISharedMemory&) = delete;

		/** Noncopyable. */
		ISharedMemory& operator = (const ISharedMemory&) = delete;

		/** Disable new. */
		void* operator new (size_t) = delete;


	public:
		ISharedMemory() noexcept {}

		~ISharedMemory()
		{
			if (handle)
			{
				if (memory && size != 0)
				{
					// Unmaps a mapped view of a file from the calling process's address space.
					// see https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-unmapviewoffile
					::UnmapViewOfFile(memory);
				}

				if (handle)
				{
					::CloseHandle(handle);
				}
			}
		}

		bool Open(wchar_t const* sharedMemoryName, UINT32 sharedMemorySize, EFlag sharedMemoryFlag)
		{
			name.Construct(sharedMemoryName);

			if (sharedMemoryFlag == EFlag::Open)
			{
				// Opens a named file mapping object.
				// see https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-openfilemappingw
				handle = ::OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, name.data);
			}
			else if (sharedMemoryFlag == EFlag::Create)
			{
				// Creates or opens a named file mapping object for a specified file.
				// see https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-createfilemappingw
				handle = ::CreateFileMappingW
					( INVALID_HANDLE_VALUE
					, security.data
					, PAGE_READWRITE | SEC_COMMIT
					, 0
					, static_cast<DWORD>(sharedMemorySize)
					, name.data
				);

				// If the object exists, remove it.
				if (handle && ::GetLastError() == ERROR_ALREADY_EXISTS)
				{
					Logger::Error("Failed to create shared memory, shared memory has existed.`");
					::CloseHandle(handle);
					handle = NULL;
				}
			}

			if (handle)
			{
				size = sharedMemorySize;
			}

			return !!handle;
		}


		void* MapMemory(UINT32* outSharedMemorySize = nullptr)
		{
			if (memory)
			{
				if (outSharedMemorySize) *outSharedMemorySize = size;
				return memory;
			}

			if (!handle)
			{
				return nullptr;
			}

			// Maps a view of a file mapping into the address space of a calling process.
			// see https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-mapviewoffile
			LPVOID pmem = ::MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (pmem == nullptr)
			{
				return nullptr;
			}

			// Checks it's a valid memory pointer.
			// see https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualquery
			MEMORY_BASIC_INFORMATION meminfo;
			if (::VirtualQuery(pmem, &meminfo, sizeof(meminfo)) == 0)
			{
				return nullptr;
			}

			memory = pmem;
			size = static_cast<UINT32>(meminfo.RegionSize);

			if (outSharedMemorySize) *outSharedMemorySize = size;
			return memory;
		}
	};
}