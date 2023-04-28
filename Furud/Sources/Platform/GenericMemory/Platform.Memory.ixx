//
// Platform.Memory.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Debug utility.
//
module;

#include <cstdlib>  
#include <crtdbg.h>



export module Furud.Platform.Memory;

export namespace Furud
{
	void MemoryLeakValidation()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}
}