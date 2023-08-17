//
// Platform.RHI.Resource.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface - Resource.
//
export module Furud.Platform.RHI.Resource;
export import :GPUFence;
export import :Buffer;
import :RefCounting;

export namespace Furud
{
	using RHIBufferRef = TRHIRefCountPtr<RHIBuffer>;
	using RHIGPUFenceRef = TRHIRefCountPtr<RHIGPUFence>;
}