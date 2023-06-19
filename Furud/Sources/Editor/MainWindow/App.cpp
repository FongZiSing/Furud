//
// App.cpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @Author FongZiSing
//
// The Warpper class of Direct3D.
//
module;

// Resource headers.
#include "Resources/framework.h"
#include "Resources/Resource.h"

// Windows header.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#define GET_X_LPARAM(lp) ((int32_t)(int16_t)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int32_t)(int16_t)HIWORD(lp))

// DirectX 12 headers.
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include "DirectXHeaders/d3dx12.h"

// DirectX math headers.
#include <DirectXColors.h>

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

// Smart Pointer Header.
#include <wrl/client.h>

// C++ Standard Library.
#include <stdint.h>
#include <string>
#include <vector>
#include <format>

// Intrin
#include <intrin.h>

// Furud headers.
#include <Furud.hpp>



module Furud.App;

import Furud.Platform.API.FrameTimer;
import Furud.Platform.RHI.Verification;
using Microsoft::WRL::ComPtr;

namespace Furud
{
	D3DApp::D3DApp()
	{
	}

	D3DApp::~D3DApp()
	{
		if (D3D12Device)
		{
			FlushD3D12CommandQueue_Internal();
		}
	}

	void D3DApp::Draw(const float deltaTime)
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
			D3D12CommandList->Reset(D3D12CommandListAllocator.Get(), nullptr),
			D3D12Device.Get(),
			"D3D12CommandList->Reset(D3D12CommandListAllocator.Get(), nullptr)"
		);

		D3D12CommandList->RSSetViewports(1, &ScreenViewport);
		D3D12CommandList->RSSetScissorRects(1, &ScissorRect);

		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView
			{
				RTVHeap->GetCPUDescriptorHandleForHeapStart(),
				numSwapChainIndex,
				RTVDescriptorSize
			};

			CD3DX12_CPU_DESCRIPTOR_HANDLE DepthStencilView
			{
				DSVHeap->GetCPUDescriptorHandleForHeapStart()
			};

			// Indicate a state transition on the resource usage.
			D3D12_RESOURCE_BARRIER beginSwapChainBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
				SwapChainBuffers[numSwapChainIndex].Get(),
				D3D12_RESOURCE_STATE_PRESENT,
				D3D12_RESOURCE_STATE_RENDER_TARGET);
			D3D12CommandList->ResourceBarrier(1, &beginSwapChainBarrier);

			// Clear the back buffer and depth buffer.
			D3D12CommandList->ClearRenderTargetView(
				CurrentBackBufferView,
				DirectX::Colors::LightSteelBlue,
				0,
				nullptr);

			D3D12CommandList->ClearDepthStencilView(
				DSVHeap->GetCPUDescriptorHandleForHeapStart(),
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
				1.0f,
				0,
				0,
				nullptr);

			// Specify the buffers we are going to render to.
			D3D12CommandList->OMSetRenderTargets(
				1,
				&CurrentBackBufferView,
				true,
				&DepthStencilView);

			// Indicate a state transition on the resource usage.
			D3D12_RESOURCE_BARRIER endSwapChainBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
				SwapChainBuffers[numSwapChainIndex].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PRESENT);
			D3D12CommandList->ResourceBarrier(1, &endSwapChainBarrier);

		}

		// Add the command list to the queue for execution.
		{
			VerifyD3D12Result(
				D3D12CommandList->Close(),
				D3D12Device.Get(),
				"D3D12CommandList->Close()"
			);
			ID3D12CommandList* allCommandLists[] = { D3D12CommandList.Get() };
			D3D12CommandQueue->ExecuteCommandLists(_countof(allCommandLists), allCommandLists);
		}

		// Swap the back and front buffers.
		DXGISwapChain->Present(0, 0);
		numSwapChainIndex = (numSwapChainIndex + 1) % numSwapChainBuffer;
		
		// Wait until frame commands are complete.  This waiting is inefficient and is
		// done for simplicity.  Later we will show how to organize our rendering code
		// so we do not have to wait per frame.
		FlushD3D12CommandQueue_Internal();
	}

	void D3DApp::EnableD3D12DebugLayer_Internal()
	{
		// Enable the D3D12 debug layer.
#if defined(_DEBUG) || defined(DEBUG)
		ComPtr<ID3D12Debug> debugController;
		VerifyD3D12Result( D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf())) );
		debugController->EnableDebugLayer();
		bEnableDebugLayer = true;
#endif
	}

	void D3DApp::CreateD3D12Device_Internal()
	{
		// Create DirectX Graphics Infrastructure Factory.
		VerifyD3D12Result(  CreateDXGIFactory1(IID_PPV_ARGS(DXGIFactory.GetAddressOf())) );

		ComPtr<IDXGIAdapter> ChosenAdapter;
		RHIAdapter::EnumAdapters(DXGIFactory, RHIAdapters, ChosenAdapter);

		// Try to create hardware device.
		HRESULT hr = D3D12CreateDevice(
			ChosenAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(D3D12Device.GetAddressOf()));

		// Fallback to WARP device.
		if (FAILED(hr))
		{
			ComPtr<IDXGIAdapter> pWarpAdapter;
			hr = DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(pWarpAdapter.GetAddressOf()));

			if (SUCCEEDED(hr))
			{
				hr = D3D12CreateDevice(
					pWarpAdapter.Get(),
					D3D_FEATURE_LEVEL_11_0,
					IID_PPV_ARGS(D3D12Device.GetAddressOf()));
			}
		}

		VerifyD3D12Result( hr );
	}

	void D3DApp::CheckD3D12FeatureSupport_Internal()
	{
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

			num4xMSAAQuality = multisampleQualityLevels.NumQualityLevels;
			assert(num4xMSAAQuality > 0 && "Unexpected MSAA quality level.");
		}
	}

	void D3DApp::CreateD3D12CommandObjects_Internal()
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			
		VerifyD3D12Result(
			D3D12Device->CreateCommandQueue(
				&queueDesc,
				IID_PPV_ARGS(D3D12CommandQueue.GetAddressOf()))
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

		// Start off in a closed state. This is because the first time we refer 
		// to the command list we will Reset it, and it needs to be closed before
		// calling Reset.
		D3D12CommandList->Close();
	}

	void D3DApp::CreateD3D12SwapChain_Internal()
	{
		// Release the previous swapchain we will be recreating.
		DXGISwapChain.Reset();

		DXGI_SWAP_CHAIN_DESC SwapChainDesc {};
		SwapChainDesc.BufferDesc.Width = width;
		SwapChainDesc.BufferDesc.Height = height;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 120;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		SwapChainDesc.BufferCount = numSwapChainBuffer;
		SwapChainDesc.OutputWindow = hWnd;
		SwapChainDesc.Windowed = true;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		VerifyD3D12Result(
			DXGIFactory->CreateSwapChain(
				D3D12CommandQueue.Get(),
				&SwapChainDesc,
				DXGISwapChain.GetAddressOf())
		);
	}

	void D3DApp::CreateD3D12DescriptorHeap_Internal()
	{
		// Descriptor Heap of render target view.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		rtvHeapDesc.NumDescriptors = numSwapChainBuffer;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;
		VerifyD3D12Result(
			D3D12Device->CreateDescriptorHeap(
				&rtvHeapDesc,
				IID_PPV_ARGS(RTVHeap.GetAddressOf()))
		);
		RTVDescriptorSize = D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		assert(RTVDescriptorSize > 0 && "Unexpected descriptor size!");

		// Descriptor Heap of depth stencil view.
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = 0;
		VerifyD3D12Result(
			D3D12Device->CreateDescriptorHeap(
			&dsvHeapDesc, IID_PPV_ARGS(DSVHeap.GetAddressOf()))
		);
		DSVDescriptorSize = D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		assert(DSVDescriptorSize > 0 && "Unexpected descriptor size!");
	}

	void D3DApp::ResizeD3D12SwapChain_Internal()
	{
		// Flush before changing any resources.
		FlushD3D12CommandQueue_Internal();

		VerifyD3D12Result(
			D3D12CommandList->Reset(
				D3D12CommandListAllocator.Get(),
				nullptr)
		);

		// Release the previous resources we will be recreating.
		for (UINT i = 0; i < numSwapChainBuffer; ++i)
		{
			SwapChainBuffers[i].Reset();
		}
		DepthStencilBuffer.Reset();
		numSwapChainIndex = 0;

		// Create the render target and view.
		{
			// Resize the swap chain.
			VerifyD3D12Result(
				DXGISwapChain->ResizeBuffers(
					numSwapChainBuffer,
					width,
					height,
					DXGI_FORMAT_R8G8B8A8_UNORM,
					DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)
			);

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
			for (UINT i = 0; i < numSwapChainBuffer; i++)
			{
				VerifyD3D12Result( DXGISwapChain->GetBuffer(i, IID_PPV_ARGS(SwapChainBuffers[i].GetAddressOf())) );
				D3D12Device->CreateRenderTargetView(SwapChainBuffers[i].Get(), nullptr, rtvHeapHandle);
				rtvHeapHandle.Offset(1, RTVDescriptorSize);
			}
		}

		// Create the depth/stencil buffer and view.
		{
			// Allocates the resource.
			D3D12_RESOURCE_DESC depthStencilDesc;
			depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			depthStencilDesc.Alignment = 0;
			depthStencilDesc.Width = width;
			depthStencilDesc.Height = height;
			depthStencilDesc.DepthOrArraySize = 1;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
			depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

			D3D12_CLEAR_VALUE clearValue;
			clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			clearValue.DepthStencil.Depth = 1.0f;
			clearValue.DepthStencil.Stencil = 0;

			D3D12_HEAP_PROPERTIES dsvHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			VerifyD3D12Result(
				D3D12Device->CreateCommittedResource(
					&dsvHeapProperties,
					D3D12_HEAP_FLAG_NONE,
					&depthStencilDesc,
					D3D12_RESOURCE_STATE_COMMON,
					&clearValue,
					IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()))
			);

			// Create the resource view.
			D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
			depthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;
			depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilViewDesc.Texture2D.MipSlice = 0;
			D3D12Device->CreateDepthStencilView(
				DepthStencilBuffer.Get(), 
				&depthStencilViewDesc,
				DSVHeap->GetCPUDescriptorHandleForHeapStart());

			// Transition the resource from its initial state to be used as a depth buffer.
			D3D12_RESOURCE_BARRIER dsvBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
				DepthStencilBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON,
				D3D12_RESOURCE_STATE_DEPTH_WRITE);
			D3D12CommandList->ResourceBarrier(1, &dsvBarrier);
		}

		// Execute the resize commands.
		{
			VerifyD3D12Result( D3D12CommandList->Close() );
			ID3D12CommandList* allCommandLists[] = { D3D12CommandList.Get() };
			D3D12CommandQueue->ExecuteCommandLists(_countof(allCommandLists), allCommandLists);
		}

		// Wait until resize is complete.
		FlushD3D12CommandQueue_Internal();

		// Update the viewport transform to cover the client area.
		{
			ScreenViewport.TopLeftX = 0;
			ScreenViewport.TopLeftY = 0;
			ScreenViewport.Width = static_cast<float>(width);
			ScreenViewport.Height = static_cast<float>(height);
			ScreenViewport.MinDepth = 0.0f;
			ScreenViewport.MaxDepth = 1.0f;
			ScissorRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
		}
	}

	void D3DApp::CreateD3D12Fence_Internal()
	{
		// Create fence.
		VerifyD3D12Result(
			D3D12Device->CreateFence(
				0,
				D3D12_FENCE_FLAG_NONE,
				IID_PPV_ARGS(D3D12Fence.GetAddressOf()))
		);
	}

	void D3DApp::FlushD3D12CommandQueue_Internal()
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
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);

			D3D12Fence->SetEventOnCompletion(CurrentFence, eventHandle);

			// Wait until the GPU hits current fence event is fired.
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}

	HRESULT D3DApp::OnCreate()
	{
		// Enable the DirectX debug layer.
		EnableD3D12DebugLayer_Internal();

		// Create DirectX Device.
		CreateD3D12Device_Internal();

		// Check DirectX features.
		CheckD3D12FeatureSupport_Internal();

		// Create command objects.
		CreateD3D12CommandObjects_Internal();

		// Create the swap chain.
		CreateD3D12SwapChain_Internal();

		// Create the descriptor heaps.
		CreateD3D12DescriptorHeap_Internal();

		// Create fence.
		CreateD3D12Fence_Internal();

		// Initialize the swap chain.
		ResizeD3D12SwapChain_Internal();

		// Dispatch create event.
		return HandleCreateEvent(width, height) ? S_OK : S_FALSE;
	}

	LRESULT D3DApp::OnActivate(const WPARAM& inFlags, const HWND& inHwnd)
	{
		// `inFlags` specifies whether the window is being activated or deactivated. It can be one of the following values.
		//     WA_ACTIVE(1), WA_CLICKACTIVE(2), WA_INACTIVE(0)
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-activate
		if (WA_INACTIVE == inFlags)
		{
			bResizing = false;
			PressingMouseNum = 0;
			ReleaseCapture();
		}
		return FALSE;
	}

	LRESULT D3DApp::OnKeyDown(const WPARAM& inKey)
	{
		// `inKey` is the virtual-key code of the nonsystem key. @see https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
		HandleKeyDownEvent(inKey);
		return FALSE;
	}

	LRESULT D3DApp::OnKeyUp(const WPARAM& inKey)
	{
		// `inKey`, the same as key down event.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keyup
		HandleKeyUpEvent(inKey);
		return FALSE;
	}

	LRESULT D3DApp::OnLeftMouseDown(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
	{
		// `inFlags`, the same as mouse move event.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-lbuttondown
		HandleLeftMouseDownEvent(inFlags, inX, inY);
		SetCapture(hWnd);
		PressingMouseNum++;
		return FALSE;
	}

	LRESULT D3DApp::OnLeftMouseUp(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
	{
		// `inFlags`, the same as mouse move event.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-lbuttonup
		HandleLeftMouseUpEvent(inFlags, inX, inY);
		ReleaseCapture();
		PressingMouseNum--;
		if (PressingMouseNum <= 0)
		{
			PressingMouseNum = 0;
			ReleaseCapture();
		}
		return FALSE;
	}

	LRESULT D3DApp::OnMiddleMouseDown(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
	{
		// `inFlags`, the same as mouse move event.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mbuttondown
		HandleMiddleMouseDownEvent(inFlags, inX, inY);
		SetCapture(hWnd);
		PressingMouseNum++;
		return FALSE;
	}

	LRESULT D3DApp::OnMiddleMouseUp(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
	{
		// `inFlags`, the same as mouse move event.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mbuttonup
		// @note that when a shortcut menu is present (displayed), coordinates are relative to the screen, not the client area.
		//     Because TrackPopupMenu is an asynchronous call and the WM_MBUTTONUP notification does not have a special flag indicating coordinate derivation,
		//     an application cannot tell if the x,y coordinates contained in lParam are relative to the screen or the client area.
		HandleMiddleMouseUpEvent(inFlags, inX, inY);
		PressingMouseNum--;
		if (PressingMouseNum <= 0)
		{
			PressingMouseNum = 0;
			ReleaseCapture();
		}
		return FALSE;
	}

	LRESULT D3DApp::OnRightMouseDown(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
	{
		// `inFlags`, the same as mouse move event.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-rbuttondown
		HandleRightMouseDownEvent(inFlags, inX, inY);
		SetCapture(hWnd);
		PressingMouseNum++;
		return FALSE;
	}

	LRESULT D3DApp::OnRightMouseUp(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
	{
		// `inFlags`, the same as mouse move event.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-rbuttonup
		HandleRightMouseUpEvent(inFlags, inX, inY);
		PressingMouseNum--;
		if (PressingMouseNum <= 0)
		{
			PressingMouseNum = 0;
			ReleaseCapture();
		}
		return FALSE;
	}

	LRESULT D3DApp::OnMouseMove(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
	{
		// `inFlags` indicates whether various virtual keys are down. This parameter can be one or more of the following values.
		//     MK_CONTROL(0x0008), MK_LBUTTON(0x0001), MK_MBUTTON(0x0010), MK_RBUTTON(0x0002),
		//     MK_SHIFT(0x0004), MK_XBUTTON1(0x0020), MK_XBUTTON2(0x0040)
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mousemove
		HandleMouseMoveEvent(inFlags, inX, inY);
		return FALSE;
	}

	LRESULT D3DApp::OnMouseWheel(const uint32_t& inFlags, const int16_t& inZDelta, const int32_t& inX, const int32_t& inY)
	{
		// `inFlags`, the same as mouse move event.
		// `zDelta` indicates the distance the wheel is rotated, expressed in multiples or divisions of WHEEL_DELTA, which is 120.
		//     A positive value indicates that the wheel was rotated forward, away from the user.
		//     A negative value indicates that the wheel was rotated backward, toward the user.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mousewheel
		HandleMouseWheelEvent(inFlags, inZDelta, inX, inY);
		return FALSE;
	}

	LRESULT D3DApp::OnSizing(const WPARAM& inFlags)
	{
		// `inFlags` is the edge of the window that is being sized. It can be one of the following values.
		//     WMSZ_BOTTOM(6), WMSZ_BOTTOMLEFT(7), WMSZ_BOTTOMRIGHT(8), WMSZ_LEFT(1),
		//     WMSZ_RIGHT(2), WMSZ_TOP(3), WMSZ_TOPLEFT(4), WMSZ_TOPRIGHT(5)
		// An application should return TRUE if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-sizing
		bResizing = true;
		HandleResizingEvent(bResizing, width, height);
		return TRUE;
	}

	LRESULT D3DApp::OnSize(const WPARAM& inFlags, const uint32_t& inWidth, const uint32_t& inHeight)
	{
		// `inFlags` is the type of resizing requested. It can be one of the following values.
		//     SIZE_MAXHIDE(4), SIZE_MAXIMIZED(2), SIZE_MAXSHOW(3), SIZE_MINIMIZED(1), SIZE_RESTORED(0)
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-size
		width = inWidth;
		height = inHeight;

		if (inFlags == SIZE_MAXIMIZED)
		{
			ResizeD3D12SwapChain_Internal();
			HandleResizeEvent(width, inHeight);
		}
		else if (inFlags == SIZE_RESTORED && bWindowMaximum)
		{
			ResizeD3D12SwapChain_Internal();
			HandleResizeEvent(width, inHeight);
		}

		bWindowMaximum = (inFlags == SIZE_MAXIMIZED);
		bWindowMinimum = (inFlags == SIZE_MINIMIZED);

		static bool localcache[2] = { !bWindowMaximum, !bWindowMinimum };

		if (localcache[0] != bWindowMaximum || localcache[1] != bWindowMinimum)
		{
			localcache[0] = bWindowMaximum;
			localcache[1] = bWindowMinimum;
			HandleMaxMinEvent(bWindowMaximum, bWindowMinimum);
		}
		return FALSE;
	}

	LRESULT D3DApp::OnExitSizeMove()
	{
		// Sent one time to a window, after it has exited the moving or sizing modal loop.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-exitsizemove
		if (bResizing)
		{
			bResizing = false;
			HandleResizingEvent(false, width, height);
			ResizeD3D12SwapChain_Internal();
			HandleResizeEvent(width, height);
		}
		return FALSE;
	}

	LRESULT D3DApp::OnDestroy()
	{
		// Sent when a window is being destroyed.
		// It is sent to the window procedure of the window being destroyed after the window is removed from the screen.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-destroy
		HandleDestroyEvent();
		PostQuitMessage(0);
		bDestory = true;
		return FALSE;
	}

	HRESULT D3DApp::Initialize(HINSTANCE hInstance, int32_t nCmdShow)
	{
		HRESULT hr = S_OK;
		LoadStringW(hInstance, IDS_APP_TITLE, wndCaption, MaxLoadString);
		LoadStringW(hInstance, IDC_FURUD, wndClass, MaxLoadString);
		appInstance = hInstance;

		// Register window class.
		if (SUCCEEDED(hr))
		{
			WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = D3DApp::WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FURUD));
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = nullptr;
			wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FURUD);
			wcex.lpszClassName = wndClass;
			wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
			RegisterClassExW(&wcex);

			RECT windowRect = { 0, 0, LONG(width), LONG(height) };
			AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
			windowRect.right -= windowRect.left;
			windowRect.bottom -= windowRect.top;
			windowRect.left = (::GetSystemMetrics(SM_CXFULLSCREEN) - windowRect.right) / 2;
			windowRect.top = (::GetSystemMetrics(SM_CYFULLSCREEN) - windowRect.bottom) / 2;
			hWnd = CreateWindowEx(
				0,
				wndClass,
				wndCaption,
				WS_OVERLAPPEDWINDOW,
				windowRect.left,
				windowRect.top,
				windowRect.right,
				windowRect.bottom,
				0,
				0,
				hInstance,
				this);

			hr = hWnd ? S_OK : E_FAIL;
		}

		if (SUCCEEDED(hr))
		{
			hr = OnCreate();
		}

		if (SUCCEEDED(hr))
		{
			ShowWindow(hWnd, nCmdShow);
			UpdateWindow(hWnd);
		}
		return hr;
	}

	int32_t D3DApp::Run()
	{
		FrameTimer timer{};
		MSG msg{};

		while (msg.message != WM_QUIT)
		{
			//elapsed.UpdateTime(timer);

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				frameCount++;
				timer.BeginFrame();
				Draw(timer.GetDeltaTime());
				timer.EndFrame();
			}
		}

		return (int32_t)msg.wParam;
	}

	LRESULT D3DApp::WndProc(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam)
	{
		static D3DApp* pThis = nullptr;

		switch (message)
		{
		furud_unlikely case WM_CREATE:
			pThis = static_cast<D3DApp*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
			break;

		furud_unlikely case WM_DESTROY:
			return pThis->OnDestroy();

		furud_unlikely case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDM_EXIT:
				return pThis->OnDestroy();
			default:
				return DefWindowProc(hwnd, message, wParam, lParam);
			}
		}


		case WM_ACTIVATE:       return pThis->OnActivate(wParam, (HWND)lParam);
		case WM_KEYDOWN:        return pThis->OnKeyDown(wParam);
		case WM_KEYUP:          return pThis->OnKeyUp(wParam);

		case WM_LBUTTONDOWN:    return pThis->OnLeftMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
		case WM_MBUTTONDOWN:    return pThis->OnMiddleMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
		case WM_RBUTTONDOWN:    return pThis->OnRightMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));

		case WM_LBUTTONUP:      return pThis->OnLeftMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));
		case WM_MBUTTONUP:      return pThis->OnMiddleMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));
		case WM_RBUTTONUP:      return pThis->OnRightMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));

		case WM_MOUSEMOVE:      return pThis->OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		case WM_MOUSEWHEEL:     return pThis->OnMouseWheel(LOWORD(wParam), HIWORD(wParam), LOWORD(lParam), HIWORD(lParam));

		case WM_SIZING:         return pThis->OnSizing(wParam);
		case WM_SIZE:           return pThis->OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
		case WM_EXITSIZEMOVE:   return pThis->OnExitSizeMove();

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		return TRUE;
	}
}