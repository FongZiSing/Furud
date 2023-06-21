//
// RHI.cpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface.
//
module;

// Windows header.
#include "RHICommon.hpp"
#include "RHIVerification.hpp"
#include "RHIDevice.hpp"
#include "RHIAdapter.hpp"
#include "RHIViewport.hpp"

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")



module Furud.Platform.RHI;

using Microsoft::WRL::ComPtr;

namespace Furud::RHI
{
	bool bEnableDebugLayer = false;
	ComPtr<IDXGIFactory4> GRHIFactory;
	RHIAdapter GRHIAdapters;
	RHIDevice GRHIDevice;
	RHIViewport GRHIViewport;

	void Init()
	{
		// Enable the D3D12 debug layer.
#if defined(_DEBUG) || defined(DEBUG)
		ComPtr<ID3D12Debug> debugController;
		VerifyD3D12Result(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf())));
		debugController->EnableDebugLayer();
		bEnableDebugLayer = true;
#endif

		// Create DirectX Graphics Infrastructure Factory.
		VerifyD3D12Result(CreateDXGIFactory1(IID_PPV_ARGS(GRHIFactory.GetAddressOf())));

		// Enumerates adapters.
		GRHIAdapters.Initialize(GRHIFactory.Get());

		GRHIDevice.Initialize(GRHIFactory.Get(), GRHIAdapters.Get());
	}

	void CreateViewport(void* windowHandle, uint32_t clientWidth, uint32_t clientHeight)
	{
		GRHIViewport.Initialize(GRHIFactory.Get(), &GRHIDevice, (HWND)windowHandle, clientWidth, clientHeight);

		ResizeViewport(clientWidth, clientHeight);
	}

	void ResizeViewport(uint32_t clientWidth, uint32_t clientHeight)
	{
		// Flush before changing any resources.
		GRHIDevice.FlushCommandQueue();

		// Reusing the command list reuses memory.
		GRHIDevice.ResetCommandList();
		
		// Generate the resize commmand.
		GRHIViewport.Resize(&GRHIDevice, clientWidth, clientHeight);
		
		// Execute the resize commands.
		GRHIDevice.ExecuteCommandList();

		// Wait until initialization is complete.
		GRHIDevice.FlushCommandQueue();
	}

	void DrawViewport()
	{
		// Reuse the memory associated with command recording.
		// We can only reset when the associated command lists have finished execution on the GPU.
		// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
		// Reusing the command list reuses memory.
		GRHIDevice.ResetCommandListAndAllocator();

		// Generate the draw commands.
		GRHIViewport.Draw(GRHIDevice.GetCommandList());

		// Execute the draw commands.
		GRHIDevice.ExecuteCommandList();

		// Swap the back and front buffers.
		GRHIViewport.Present();

		// Wait until frame commands are complete.  This waiting is inefficient and is
		// done for simplicity.  Later we will show how to organize our rendering code
		// so we do not have to wait per frame.
		GRHIDevice.FlushCommandQueue();

	}
	void FlushCommandQueue()
	{
		GRHIDevice.FlushCommandQueue();
	}
}