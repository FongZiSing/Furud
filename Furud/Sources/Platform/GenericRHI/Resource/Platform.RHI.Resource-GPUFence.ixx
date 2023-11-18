//
// Platform.RHI.Resource-GPUFence.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface - GPU Fence.
//
module;

#include "../RHICommon.hpp"


export module Furud.Platform.RHI.Resource:GPUFence;
import :Common;
import Furud.Platform.RHI.Verification;

export namespace Furud
{
	class RHIGPUFence : public IRHIResource
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Fence> fence;

		UINT64 value = 0;


	protected:
		void InitRHI(RHIResourceContext&& context, UINT64 initValue);


	public:
		RHIGPUFence() = delete;

		RHIGPUFence(RHIResourceContext&& context, unsigned long long initValue = 0)
			: IRHIResource(ERHIResourceType::Fence)
		{
			InitRHI(std::forward<RHIResourceContext>(context), initValue);
		}

		void SignalWait(RHIResourceContext&& context);
	};
}



namespace Furud
{
	void RHIGPUFence::InitRHI(RHIResourceContext&& context, UINT64 initValue)
	{
		auto&& [D3D12Device, D3D12CommandQueue, D3D12CommandList] = context;
		value = initValue;
		VerifyD3D12Result(
			D3D12Device->CreateFence(
				initValue,
				D3D12_FENCE_FLAG_NONE,
				IID_PPV_ARGS(fence.GetAddressOf()))
		);
	}

	void RHIGPUFence::SignalWait(RHIResourceContext&& context)
	{
		auto&& [D3D12Device, D3D12CommandQueue, D3D12CommandList] = context;

		// Advance the fence value to mark commands up to this fence point.
		value++;

		// Add an instruction to the command queue to set a new fence point.  Because we 
		// are on the GPU timeline, the new fence point won't be set until the GPU finishes
		// processing all the commands prior to this Signal().
		D3D12CommandQueue->Signal(fence.Get(), value);

		// Wait until the GPU has completed commands up to this fence point.
		if (fence->GetCompletedValue() < value)
		{
			HANDLE eventHandle = ::CreateEventExW(nullptr, nullptr, 0, EVENT_ALL_ACCESS);

			fence->SetEventOnCompletion(value, eventHandle);

			// Wait until the GPU hits current fence event is fired.
			::WaitForSingleObject(eventHandle, INFINITE);
			::CloseHandle(eventHandle);
		}
	}
}