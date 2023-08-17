//
// Platform.RHI.Device.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface - Device.
//
module;

#include "../RHICommon.hpp"


export module Furud.Platform.RHI.Device;
import Furud.Platform.RHI.Verification;

export namespace Furud
{
	struct RHIFeatures
	{
		UINT num4xMSAAQuality = 0;
	};



	class RHIDevice
	{
	private:
		RHIFeatures features;

		IDXGIAdapter* LinkedAdapter;

		Microsoft::WRL::ComPtr<ID3D12Device> D3D12Device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> D3D12CommandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> D3D12CommandListAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> D3D12CommandList;

		Microsoft::WRL::ComPtr<ID3D12Fence> D3D12Fence;
		UINT64 CurrentFence = 0;


	public:
		furud_inline ID3D12Device* GetDevice() { return D3D12Device.Get(); }
		furud_inline ID3D12CommandQueue* GetCommandQueue() { return D3D12CommandQueue.Get(); }
		furud_inline ID3D12GraphicsCommandList* GetCommandList() { return D3D12CommandList.Get(); }


	public:
		void Initialize(IDXGIFactory4* DXGIFactory, IDXGIAdapter* DXGIAdapter);

		void FlushCommandQueue();

		void ResetCommandListAndAllocator(ID3D12PipelineState* PipelineState);

		void ResetCommandList();

		void ExecuteCommandList();
	};
}



namespace Furud
{
	void RHIDevice::Initialize(IDXGIFactory4* DXGIFactory, IDXGIAdapter* DXGIAdapter)
	{
		// Try to create D3D12 device based on the specified adapter.
		LinkedAdapter = DXGIAdapter;
		{
			// Try to create hardware device.
			HRESULT hr = D3D12CreateDevice(
				LinkedAdapter,
				D3D_FEATURE_LEVEL_12_0,
				IID_PPV_ARGS(D3D12Device.GetAddressOf()));

			// Fallback to WARP device.
			if (FAILED(hr))
			{
				Microsoft::WRL::ComPtr<IDXGIAdapter> pWarpAdapter;
				hr = DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(pWarpAdapter.GetAddressOf()));

				if (SUCCEEDED(hr))
				{
					hr = D3D12CreateDevice(
						pWarpAdapter.Get(),
						D3D_FEATURE_LEVEL_12_0,
						IID_PPV_ARGS(D3D12Device.GetAddressOf()));
				}
			}

			VerifyD3D12Result(hr);
		}

		// Check 4X MSAA quality support for our back buffer format.
		// All Direct3D 11 capable devices support 4X MSAA for all render 
		// target formats, so we only need to check quality support.
		{
			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS multisampleQualityLevels;
			multisampleQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			multisampleQualityLevels.SampleCount = 4;
			multisampleQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
			multisampleQualityLevels.NumQualityLevels = 0;
			D3D12Device->CheckFeatureSupport(
				D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
				&multisampleQualityLevels,
				sizeof(multisampleQualityLevels));

			features.num4xMSAAQuality = multisampleQualityLevels.NumQualityLevels;
			assert(features.num4xMSAAQuality > 0 && "Unexpected MSAA quality level.");
		}

		// Create command objects.
		{
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

			VerifyD3D12Result(
				D3D12Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(D3D12CommandQueue.GetAddressOf())),
				D3D12Device.Get(),
				"D3D12Device->CreateCommandQueue"
			);

			VerifyD3D12Result(
				D3D12Device->CreateCommandAllocator(
					D3D12_COMMAND_LIST_TYPE_DIRECT,
					IID_PPV_ARGS(D3D12CommandListAllocator.GetAddressOf()))
			);

			VerifyD3D12Result(
				D3D12Device->CreateCommandList(
					0,
					D3D12_COMMAND_LIST_TYPE_DIRECT,
					D3D12CommandListAllocator.Get(), // Associated command allocator
					nullptr,                         // Initial PipelineStateObject
					IID_PPV_ARGS(D3D12CommandList.GetAddressOf()))
			);
		}

		// Start off in a closed state. This is because the first time we refer 
		// to the command list we will Reset it, and it needs to be closed before
		// calling Reset.
		D3D12CommandList->Close();

		// Create fence.
		{
			VerifyD3D12Result(
				D3D12Device->CreateFence(
					CurrentFence, // Initial value 0.
					D3D12_FENCE_FLAG_NONE,
					IID_PPV_ARGS(D3D12Fence.GetAddressOf()))
			);
		}
	}

	void RHIDevice::FlushCommandQueue()
	{
		// Advance the fence value to mark commands up to this fence point.
		CurrentFence++;

		// Add an instruction to the command queue to set a new fence point.  Because we 
		// are on the GPU timeline, the new fence point won't be set until the GPU finishes
		// processing all the commands prior to this Signal().
		D3D12CommandQueue->Signal(D3D12Fence.Get(), CurrentFence);

		// Wait until the GPU has completed commands up to this fence point.
		if (D3D12Fence->GetCompletedValue() < CurrentFence)
		{
			HANDLE eventHandle = ::CreateEventExW(nullptr, nullptr, 0, EVENT_ALL_ACCESS);

			D3D12Fence->SetEventOnCompletion(CurrentFence, eventHandle);

			// Wait until the GPU hits current fence event is fired.
			::WaitForSingleObject(eventHandle, INFINITE);
			::CloseHandle(eventHandle);
		}
	}

	void RHIDevice::ResetCommandListAndAllocator(ID3D12PipelineState* PipelineState)
	{
		// Reuse the memory associated with command recording.
		// We can only reset when the associated command lists have finished execution on the GPU.
		VerifyD3D12Result(
			D3D12CommandListAllocator->Reset(),
			D3D12Device.Get(),
			"D3D12CommandListAllocator->Reset()"
		);

		// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
		// Reusing the command list reuses memory.
		VerifyD3D12Result(
			D3D12CommandList->Reset(D3D12CommandListAllocator.Get(), PipelineState),
			D3D12Device.Get(),
			"D3D12CommandList->Reset(D3D12CommandListAllocator.Get(), nullptr)"
		);
	}

	void RHIDevice::ResetCommandList()
	{
		// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
		// Reusing the command list reuses memory.
		VerifyD3D12Result(
			D3D12CommandList->Reset(D3D12CommandListAllocator.Get(), nullptr),
			D3D12Device.Get(),
			"D3D12CommandList->Reset(D3D12CommandListAllocator.Get(), nullptr)"
		);
	}

	void RHIDevice::ExecuteCommandList()
	{
		// Execute the commands.
		VerifyD3D12Result(D3D12CommandList->Close());
		ID3D12CommandList* allCommandLists[] = { D3D12CommandList.Get() };
		D3D12CommandQueue->ExecuteCommandLists(_countof(allCommandLists), allCommandLists);
	}
}