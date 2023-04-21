//
// App.hpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// TODO: Repalced with Furud-DUI.
//
#pragma once

// Windows Header.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

// D2D1 Runtimer header.
#include <dxgi1_4.h>
#include <D3D11.h>
#include <d2d1_3.h>

// Smart Pointer Header.
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include <GenericCommon.hpp>



namespace Furud
{
	class D2DApp
	{
	public:
		D2DApp();

		virtual ~D2DApp();


	protected:
		//--------------------------------
		//~ Begin override message handle.

		virtual bool HandleCreateEvent(uint32 width, uint32 height) { return false; }

		virtual void HandleActivateEvent(WPARAM flags, HWND hwnd) {}

		virtual void HandleKeyDownEvent(WPARAM inKey) {}

		virtual void HandleKeyUpEvent(WPARAM inKey) {}

		virtual void HandleLeftMouseDownEvent(WPARAM flags, int32 x, int32 y) {}

		virtual void HandleLeftMouseUpEvent(WPARAM flags, int32 x, int32 y) {}

		virtual void HandleMiddleMouseDownEvent(WPARAM flags, int32 x, int32 y) {}

		virtual void HandleMiddleMouseUpEvent(WPARAM flags, int32 x, int32 y) {}

		virtual void HandleRightMouseDownEvent(WPARAM flags, int32 x, int32 y) {}

		virtual void HandleRightMouseUpEvent(WPARAM flags, int32 x, int32 y) {}

		virtual void HandleMouseMoveEvent(WPARAM flags, int32 x, int32 y) {}

		virtual void HandleMouseWheelEvent(uint32 flags, int16 zDelta, int32 x, int32 y) {}

		virtual void HandleResizeEvent(uint32 width, uint32 height) {}

		virtual void HandleResizingEvent(bool bResizing, uint32 width, uint32 height) {}

		virtual void HandleMaxMinEvent(bool bMaximum, bool bMinimum) {}

		virtual void HandleDestroyEvent() {}

		//~ End override message handle.
		//--------------------------------


	private:
		// Initialize device-independent resources.
		HRESULT CreateDeviceIndependentResources();

		// Initialize device-dependent resources.
		HRESULT CreateDeviceResources();

		// Device-dependent resources when window resize.
		void CreateWindowSizeDependentResources();

		// Text resources.
		HRESULT CreateDrawTextResources();

		// Called every frames.
		bool BeginDraw();

		// Called every frames.
		void EndDraw();


	private:
		//--------------------------------
		//~ Begin internal message handle.

		HRESULT OnCreate();

		LRESULT OnActivate(const WPARAM& inFlags, const HWND& inHwnd);

		LRESULT OnKeyDown(const WPARAM& inKey);

		LRESULT OnKeyUp(const WPARAM& inKey);

		LRESULT OnLeftMouseDown(const WPARAM& inFlags, const int32& inX, const int32& inY);

		LRESULT OnLeftMouseUp(const WPARAM& inFlags, const int32& inX, const int32& inY);

		LRESULT OnMiddleMouseDown(const WPARAM& inFlags, const int32& inX, const int32& inY);

		LRESULT OnMiddleMouseUp(const WPARAM& inFlags, const int32& inX, const int32& inY);

		LRESULT OnRightMouseDown(const WPARAM& inFlags, const int32& inX, const int32& inY);

		LRESULT OnRightMouseUp(const WPARAM& inFlags, const int32& inX, const int32& inY);

		LRESULT OnMouseMove(const WPARAM& inFlags, const int32& inX, const int32& inY);

		LRESULT OnMouseWheel(const uint32& inFlags, const int16& inZDelta, const int32& inX, const int32& inY);

		LRESULT OnSizing(const WPARAM& inFlags);

		LRESULT OnSize(const WPARAM& inFlags, const uint32& inWidth, const uint32& inHeight);

		LRESULT OnExitSizeMove();

		LRESULT OnDestroy();

		//~ End internal message handle.
		//--------------------------------


	public:
		// Register the window classand call methods for instantiating drawing resources.
		HRESULT Initialize(HINSTANCE hInstance, int32 nCmdShow);

		// Process and dispatch messages.
		int32 Run();

		// Processes messages for the main window.
		static LRESULT CALLBACK WndProc(HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam);


	private:
		static const int32 MaxLoadString = 100;


	protected:
		wchar wndCaption[MaxLoadString] = L"";

		wchar wndClass[MaxLoadString] = L"";

		HINSTANCE appInstance = nullptr;

		HWND      hWnd = nullptr;

		uint32    width = 700;

		uint32    height = 700;

		uint64    frameCount = 0;


	private:
		//--------------------------------
		//~ Begin D2D

		D2D1_RECT_U                 rect = { 0, 0, width, height };

		ComPtr<ID3D11Device>        ptrD3DDevice;

		ComPtr<ID3D11DeviceContext> ptrD3DDeviceContext;

		ComPtr<ID2D1Factory4>       ptrD2DFactory;

		ComPtr<ID2D1Device3>        ptrD2DDevice;

		ComPtr<ID2D1DeviceContext3> ptrD2DDeviceContext;

		ComPtr<ID2D1Bitmap1>        ptrD2DTargetBimtap;

		ComPtr<ID2D1Bitmap1>        ptrD2DRenderTarget;

		ComPtr<IDXGISwapChain1>     ptrSwapChain;

		ComPtr<IDWriteFactory>      ptrWriteFactory;

		ComPtr<IDWriteTextFormat>   ptrWriteTextFormat;

		D2D1_RECT_F                 textLayoutRectFPS;

		ComPtr<ID2D1SolidColorBrush> ptrBrush;

		D3D_FEATURE_LEVEL           featureLevel;

		DXGI_PRESENT_PARAMETERS     parameters;

		//~ End D2D
		//--------------------------------


		//--------------------------------
		//~ Begin operation.

		bool   bDestory = false;

		bool   bResizing = false;

		bool   bWindowMaximum = false;

		bool   bWindowMinimum = false;

		uint32 PressingMouseNum = 0;

		//~ End operation.
		//--------------------------------
	};
}
