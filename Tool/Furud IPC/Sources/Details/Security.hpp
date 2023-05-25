//
// Security.hpp
//
//       Copyright (c) Furud IPC Library. All rights reserved.
//       @Author FongZiSing
//
// Implemention of security attributes.
//
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>



namespace Furud::IPC
{
	struct ISecurity
	{
	public:
		/** A pointer of security attributes on Windows. */
		LPSECURITY_ATTRIBUTES data;


	private:
		struct Initializer
		{
			SECURITY_ATTRIBUTES attributes {};
			SECURITY_DESCRIPTOR descriptor {};
			LPSECURITY_ATTRIBUTES pAttributes = nullptr;

			Initializer()
			{
				// Initializes a new security descriptor.
				// see https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-initializesecuritydescriptor
				if (!::InitializeSecurityDescriptor(&descriptor, SECURITY_DESCRIPTOR_REVISION))
				{
					return;
				}
				// Sets information in a discretionary access control list (DACL).
				// see https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-setsecuritydescriptordacl
				if (!::SetSecurityDescriptorDacl(&attributes, TRUE, NULL, FALSE))
				{
					return;
				}

				attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
				attributes.bInheritHandle = FALSE;
				attributes.lpSecurityDescriptor = &descriptor;
				pAttributes = &attributes;
			}

		};


	public:
		ISecurity()
		{
			static Initializer initializer;
			data = initializer.pAttributes;
		}
	};
}
