#include <pch.h>
#include <framework.h>
#include <Includes/IPC.h>

#include "Details/Semaphore.hpp"
#include <string>
#include "Client.hpp"

#include <iostream>



void TestIPC(int argc, char** argv)
{
	namespace IPC = Furud::IPC;

	if (argc < 2) return;

	std::string mode {argv[1]};
	if (mode == "server")
	{
		IPC::Server server;
		server.Init(L"client", L"server");
		std::string input;
		while (std::cin >> input)
		{
			server.Send((unsigned char*)input.c_str(), (int)input.size());
		}
	}
	else
	{
		IPC::Client server;
		server.Connect(L"client", L"server");
		char input[IPC::SHM_SIZE] = {};
		while (1)
		{
			if (server.Recv(input, IPC::SHM_SIZE))
			{
				std::string i = input;
				if (!i.empty())
				{
					std::cout << i << '\n';
				}
			}
		}
	}
	//semaphore.Wait(5000);
}