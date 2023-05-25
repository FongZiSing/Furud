#include <pch.h>
#include <framework.h>
#include <Includes/IPC.h>

#include "Details/Semaphore.hpp"
#include <string>



void TestIPC(int argc, char** argv)
{
	if (argc < 2) return;

	std::string mode {argv[1]};
	Furud::IPC::ISemaphore semaphore;
	if (mode == "create")
	{
		semaphore.Open(L"TestSem", 1, Furud::IPC::ISemaphore::EFlag::Create);
	}
	else
	{
		semaphore.Open(L"TestSem", 1, Furud::IPC::ISemaphore::EFlag::Open);
	}

	semaphore.Wait();
	//semaphore.Wait(5000);
}