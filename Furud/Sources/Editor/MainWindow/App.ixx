//
// App.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// TODO: Repalced with Furud-DUI.
//
module;

// Windows Header.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

// DirectX 12 header.
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include "DirectXHeaders/d3dx12.h"

// Smart Pointer Header.
#include <wrl/client.h>

// C++ Standard Library.
#include <stdint.h>



export module Furud.App;

using Microsoft::WRL::ComPtr;



export namespace Furud
{
	class D3DApp
	{
	public:
		D3DApp();

		virtual ~D3DApp();


	protected:
		virtual void Draw(const float deltaTime);


	protected:
		//--------------------------------
		//~ Begin override message handle.

		virtual bool HandleCreateEvent(uint32_t width, uint32_t height) { return false; }

		virtual void HandleActivateEvent(WPARAM flags, HWND hwnd) {}

		virtual void HandleKeyDownEvent(WPARAM inKey) {}

		virtual void HandleKeyUpEvent(WPARAM inKey) {}

		virtual void HandleLeftMouseDownEvent(WPARAM flags, int32_t x, int32_t y) {}

		virtual void HandleLeftMouseUpEvent(WPARAM flags, int32_t x, int32_t y) {}

		virtual void HandleMiddleMouseDownEvent(WPARAM flags, int32_t x, int32_t y) {}

		virtual void HandleMiddleMouseUpEvent(WPARAM flags, int32_t x, int32_t y) {}

		virtual void HandleRightMouseDownEvent(WPARAM flags, int32_t x, int32_t y) {}

		virtual void HandleRightMouseUpEvent(WPARAM flags, int32_t x, int32_t y) {}

		virtual void HandleMouseMoveEvent(WPARAM flags, int32_t x, int32_t y) {}

		virtual void HandleMouseWheelEvent(uint32_t flags, int16_t zDelta, int32_t x, int32_t y) {}

		virtual void HandleResizeEvent(uint32_t width, uint32_t height) {}

		virtual void HandleResizingEvent(bool bResizing, uint32_t width, uint32_t height) {}

		virtual void HandleMaxMinEvent(bool bMaximum, bool bMinimum) {}

		virtual void HandleDestroyEvent() {}

		//~ End override message handle.
		//--------------------------------


	private:
		void EnableD3D12DebugLayer_Internal();

		void CreateD3D12Device_Internal();
		
		void CheckD3D12FeatureSupport_Internal();
		
		void EnumerateD3D12Adapter_Internal();
		
		void CreateD3D12CommandObjects_Internal();
		
		void CreateD3D12SwapChain_Internal();
		
		void CreateD3D12DescriptorHeap_Internal();
		
		void ResizeD3D12SwapChain_Internal();

		void CreateD3D12Fence_Internal();

		void FlushD3D12CommandQueue_Internal();


	private:
		//--------------------------------
		//~ Begin internal message handle.

		HRESULT OnCreate();

		LRESULT OnActivate(const WPARAM& inFlags, const HWND& inHwnd);

		LRESULT OnKeyDown(const WPARAM& inKey);

		LRESULT OnKeyUp(const WPARAM& inKey);

		LRESULT OnLeftMouseDown(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY);

		LRESULT OnLeftMouseUp(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY);

		LRESULT OnMiddleMouseDown(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY);

		LRESULT OnMiddleMouseUp(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY);

		LRESULT OnRightMouseDown(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY);

		LRESULT OnRightMouseUp(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY);

		LRESULT OnMouseMove(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY);

		LRESULT OnMouseWheel(const uint32_t& inFlags, const int16_t& inZDelta, const int32_t& inX, const int32_t& inY);

		LRESULT OnSizing(const WPARAM& inFlags);

		LRESULT OnSize(const WPARAM& inFlags, const uint32_t& inWidth, const uint32_t& inHeight);

		LRESULT OnExitSizeMove();

		LRESULT OnDestroy();

		//~ End internal message handle.
		//--------------------------------


	public:
		// Register the window classand call methods for instantiating drawing resources.
		HRESULT Initialize(HINSTANCE hInstance, int32_t nCmdShow);

		// Process and dispatch messages.
		int32_t Run();

		// Processes messages for the main window.
		static LRESULT CALLBACK WndProc(HWND hWnd, uint32_t message, WPARAM wParam, LPARAM lParam);


	private:
		static const int32_t MaxLoadString = 100;


	protected:
		wchar_t wndCaption[MaxLoadString] = L"";

		wchar_t wndClass[MaxLoadString] = L"";

		HINSTANCE appInstance = nullptr;

		HWND      hWnd = nullptr;

		uint32_t    width = 720;

		uint32_t    height = 480;

		uint64_t    frameCount = 0;


	private:
		//--------------------------------
		//~ Begin D3D

		bool bEnableDebugLayer = false;
		UINT num4xMSAAQuality = 0;


		ComPtr<IDXGIFactory4> DXGIFactory;
		ComPtr<IDXGISwapChain> DXGISwapChain;


		ComPtr<ID3D12Device> D3D12Device;
		ComPtr<ID3D12CommandQueue> D3D12CommandQueue;
		ComPtr<ID3D12CommandAllocator> D3D12CommandListAllocator;
		ComPtr<ID3D12GraphicsCommandList> D3D12CommandList;


		D3D12_VIEWPORT ScreenViewport;
		D3D12_RECT ScissorRect;

		static const UINT numSwapChainBuffer = 3;
		INT numSwapChainIndex = 0;
		
		UINT RTVDescriptorSize = 0;
		ComPtr<ID3D12DescriptorHeap> RTVHeap; // Descriptor heap of render target view.
		
		UINT DSVDescriptorSize = 0;
		ComPtr<ID3D12DescriptorHeap> DSVHeap; // Descriptor heap of depth stencil view.
		
		ComPtr<ID3D12Resource> SwapChainBuffers[numSwapChainBuffer];
		ComPtr<ID3D12Resource> DepthStencilBuffer;


		ComPtr<ID3D12Fence> D3D12Fence;
		UINT64 CurrentFence = 0;

		//~ End D3D
		//--------------------------------


	private:
		//--------------------------------
		//~ Begin operation.

		bool   bDestory = false;

		bool   bResizing = false;

		bool   bWindowMaximum = false;

		bool   bWindowMinimum = false;

		uint32_t PressingMouseNum = 0;

		//~ End operation.
		//--------------------------------
	};
}
