#pragma once


#include "Details/Connectable.hpp"
#include "Details/Semaphore.hpp"
#include "Details/SharedMemory.hpp"
#include <utility>



namespace Furud::IPC
{
	class Client
	{
		static constexpr int nameCapacity = 16;
		wchar_t clientName[nameCapacity];
		wchar_t serverName[nameCapacity];

		ISemaphore ReadSemaphore;
		ISemaphore WriteSemaphore;

		ISharedMemory ReadSharedMemory;
		ISharedMemory WriteSharedMemory;
		

	private:
		/** Noncopyable. */
		Client(const Client&) = delete;

		/** Noncopyable. */
		Client& operator = (const Client&) = delete;

		/** Disable new. */
		void* operator new (size_t) = delete;


	private:
		FORCEINLINE static void InitializeName(wchar_t const* inputName, wchar_t* outputName, size_t outputNameSize) noexcept
		{
			const auto fillSize = outputNameSize - 1;
			const auto nameSize = __builtin_wcslen(inputName);
			const auto usedSize = std::min(fillSize, nameSize);
			::wmemcpy(outputName, inputName, usedSize);
			outputName[usedSize] = '\0';
		}


	public:
		Client() noexcept {}

		~Client()
		{

		}

		bool Connect(wchar_t const* inClientName, wchar_t const* inServerName)
		{
			InitializeName(inClientName, clientName, nameCapacity);
			InitializeName(inServerName, serverName, nameCapacity);
			 
			ReadSemaphore.Open(clientName, 1, ISemaphore::EFlag::Open);
			WriteSemaphore.Open(serverName, 1, ISemaphore::EFlag::Open);

			ReadSharedMemory.Open(clientName, SHM_SIZE, ISharedMemory::EFlag::Open);
			WriteSharedMemory.Open(serverName, SHM_SIZE, ISharedMemory::EFlag::Open);
			return true;
		}

		void Send(unsigned char* data, unsigned int size)
		{
			while (WriteSemaphore.Post());

			{
				unsigned char* buffer = (unsigned char*)WriteSharedMemory.MapMemory();
				::memcpy(buffer, data, size);
				WriteSemaphore.Wait(5000);
			}
			//else
			{
				// TODO
				// Ignore message.
			}
		}

		bool Recv(void* data, unsigned int size)
		{
			if (ReadSemaphore.Wait(1000))
			{
				unsigned char* buffer = (unsigned char*)ReadSharedMemory.MapMemory();
				::memcpy(data, buffer, size);
				ReadSemaphore.Post();
				return true;
			}
			else
			{
				// TODO
				// Ignore message.
			}
			return false;
		}

	};
}



namespace Furud::IPC
{
	class Server
	{
		static constexpr int nameCapacity = 16;
		wchar_t clientName[nameCapacity];
		wchar_t serverName[nameCapacity];

		ISemaphore ReadSemaphore;
		ISemaphore WriteSemaphore;

		ISharedMemory ReadSharedMemory;
		ISharedMemory WriteSharedMemory;


	private:
		/** Noncopyable. */
		Server(const Server&) = delete;

		/** Noncopyable. */
		Server& operator = (const Server&) = delete;

		/** Disable new. */
		void* operator new (size_t) = delete;


	private:
		FORCEINLINE static void InitializeName(wchar_t const* inputName, wchar_t* outputName, size_t outputNameSize) noexcept
		{
			const auto fillSize = outputNameSize - 1;
			const auto nameSize = __builtin_wcslen(inputName);
			const auto usedSize = std::min(fillSize, nameSize);
			::wmemcpy(outputName, inputName, usedSize);
			outputName[usedSize] = '\0';
		}


	public:
		Server() noexcept {}

		~Server()
		{

		}

		bool Init(wchar_t const* inClientName, wchar_t const* inServerName)
		{
			InitializeName(inClientName, clientName, nameCapacity);
			InitializeName(inServerName, serverName, nameCapacity);

			ReadSemaphore.Open(serverName, 1, ISemaphore::EFlag::Create);
			WriteSemaphore.Open(clientName, 1, ISemaphore::EFlag::Create);

			ReadSharedMemory.Open(serverName, SHM_SIZE, ISharedMemory::EFlag::Create);
			WriteSharedMemory.Open(clientName, SHM_SIZE, ISharedMemory::EFlag::Create);
			return true;
		}

		void Send(unsigned char* data, unsigned int size)
		{
			if (WriteSemaphore.Wait(1000))
			{
				unsigned char* buffer = (unsigned char*)WriteSharedMemory.MapMemory();
				::memcpy(buffer, data, size);
				WriteSemaphore.Post();
			}
			else
			{
				// TODO
				// Ignore message.
			}
		}

		void Recv(void* data, unsigned int size)
		{
			if (ReadSemaphore.Wait(1000))
			{
				unsigned char* buffer = (unsigned char*)ReadSharedMemory.MapMemory();
				::memcpy(data, buffer, size);
				ReadSemaphore.Post();
			}
			else
			{
				// TODO
				// Ignore message.
			}
		}
	};
}