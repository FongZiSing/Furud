//
// RHI.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface.
//
module;

// C++ Standard Library.
#include <stdint.h>

// Furud headers.
#include <Furud.hpp>



export module Furud.Platform.RHI;

namespace Furud::RHI
{
	export void Init();

	export void CreateViewport(void* windowHandle, uint32_t clientWidth, uint32_t clientHeight);

	export void ResizeViewport(uint32_t clientWidth, uint32_t clientHeight);

	export void DrawViewport();

	export void FlushCommandQueue();
}