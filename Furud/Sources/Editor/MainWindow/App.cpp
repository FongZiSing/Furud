//
// App.cpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @Author FongZiSing
//
// The Warpper class of Direct2D.
//
module;

// Windows header.
#include "Resources/framework.h"
#include "Resources/Resource.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

// D2D1 Runtimer header.
#include <dxgi1_4.h>
#include <D3D11.h>
#include <d2d1_3.h>

// Smart Pointer Header.
#include <wrl/client.h>

#include <dwrite.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib" )
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Dwrite.lib")

#define GET_X_LPARAM(lp) ((int32_t)(int16_t)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int32_t)(int16_t)HIWORD(lp))

#include <Furud.hpp>
#include <stdint.h>



module Furud.App;

import Furud.Platform.API.FrameTimer;
using Microsoft::WRL::ComPtr;



namespace Furud
{
	D2DApp::D2DApp()
	{
		featureLevel = D3D_FEATURE_LEVEL_1_0_CORE; // just default value, not be used.
		parameters.DirtyRectsCount = 0;
		parameters.pDirtyRects = nullptr;
		parameters.pScrollRect = nullptr;
		parameters.pScrollOffset = nullptr;
		textLayoutRectFPS = D2D1::RectF();
	}

	D2DApp::~D2DApp()
	{
	}

	HRESULT D2DApp::CreateDeviceIndependentResources()
	{
		// Create a Direct2D factory.
		HRESULT hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			reinterpret_cast<void**>(ptrD2DFactory.GetAddressOf())
		);

		return hr;
	}

	HRESULT D2DApp::CreateDeviceResources()
	{
		ComPtr<IDXGIDevice1> pDxgiDevice;
		HRESULT hr = S_OK;

		{
			uint32_t creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
			creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			D3D_FEATURE_LEVEL featureLevels[] = {
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};

			hr = D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				creationFlags,
				featureLevels,
				ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION,
				ptrD3DDevice.GetAddressOf(),
				&featureLevel,
				ptrD3DDeviceContext.GetAddressOf());
		}


		if (SUCCEEDED(hr))
		{
			hr = ptrD3DDevice->QueryInterface(IID_PPV_ARGS(pDxgiDevice.GetAddressOf()));
		}

		if (SUCCEEDED(hr))
		{
			hr = ptrD2DFactory->CreateDevice(pDxgiDevice.Get(), ptrD2DDevice.GetAddressOf());
		}

		if (SUCCEEDED(hr))
		{
			hr = ptrD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, ptrD2DDeviceContext.GetAddressOf());
		}

		return hr;
	}

	void D2DApp::CreateWindowSizeDependentResources()
	{
		ComPtr<IDXGIDevice1> pDxgiDevice;
		ComPtr<IDXGIAdapter> pDxgiAdapter;
		ComPtr<IDXGIFactory2> pDxgiFactory;
		ComPtr<IDXGISurface> pDxgiBackBuffer;

		HRESULT hr = S_OK;
		ptrD2DDeviceContext->SetTarget(nullptr);
		ptrD2DTargetBimtap.Reset();
		ptrD2DRenderTarget.Reset();
		ptrD3DDeviceContext->Flush();

		if (ptrSwapChain != nullptr)
		{
			hr = ptrSwapChain->ResizeBuffers(
				2, // Double-buffered swap chain.
				width,
				height,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				0);
			assert(hr == S_OK);
		}
		else
		{
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
			swapChainDesc.Width = width;
			swapChainDesc.Height = height;
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.Stereo = false;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
			swapChainDesc.Flags = 0;
			swapChainDesc.Scaling = DXGI_SCALING_NONE;
			swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;


			if (SUCCEEDED(hr))
			{
				hr = ptrD3DDevice->QueryInterface(IID_PPV_ARGS(pDxgiDevice.GetAddressOf()));
			}

			if (SUCCEEDED(hr))
			{
				hr = pDxgiDevice->GetAdapter(pDxgiAdapter.GetAddressOf());
			}

			if (SUCCEEDED(hr))
			{
				hr = pDxgiAdapter->GetParent(IID_PPV_ARGS(pDxgiFactory.GetAddressOf()));
			}

			if (SUCCEEDED(hr))
			{
				hr = pDxgiFactory->CreateSwapChainForHwnd(
					ptrD3DDevice.Get(),
					hWnd,
					&swapChainDesc,
					nullptr,
					nullptr,
					ptrSwapChain.GetAddressOf());
			}

			if (SUCCEEDED(hr))
			{
				hr = pDxgiDevice->SetMaximumFrameLatency(1);
			}
		}


		if (SUCCEEDED(hr))
		{
			hr = ptrSwapChain->SetRotation(DXGI_MODE_ROTATION_IDENTITY);
		}

		if (SUCCEEDED(hr))
		{
			hr = ptrSwapChain->GetBuffer(0, IID_PPV_ARGS(pDxgiBackBuffer.GetAddressOf()));
		}

		if (SUCCEEDED(hr))
		{
			D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

			hr = ptrD2DDeviceContext->CreateBitmapFromDxgiSurface(
				pDxgiBackBuffer.Get(),
				&properties,
				ptrD2DTargetBimtap.GetAddressOf());
		}

		if (SUCCEEDED(hr))
		{
			ptrD2DDeviceContext->SetTarget(ptrD2DTargetBimtap.Get());
		}

		if (SUCCEEDED(hr))
		{
			ptrD2DDeviceContext->CreateBitmap(
				D2D1::SizeU(width, height),
				D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
				reinterpret_cast<ID2D1Bitmap**>(ptrD2DRenderTarget.GetAddressOf())
			);
		}
	}

	HRESULT D2DApp::CreateDrawTextResources()
	{
		HRESULT hr = S_OK;

		{
			hr = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(ptrWriteFactory.GetAddressOf())
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = ptrWriteFactory->CreateTextFormat(
				L"Calibri",                      // Font family name
				NULL,                             // Font collection(NULL sets it to the system font collection)
				DWRITE_FONT_WEIGHT_REGULAR,       // Weight
				DWRITE_FONT_STYLE_NORMAL,         // Style
				DWRITE_FONT_STRETCH_NORMAL,       // Stretch
				16.0f,                            // Size    
				L"en-us",                         // Local
				ptrWriteTextFormat.GetAddressOf() // Pointer to recieve the created object
			);

			textLayoutRectFPS = D2D1::RectF(12.f, 12.f, 152.f, 63.f);
		}

		if (SUCCEEDED(hr))
		{
			hr = ptrD2DDeviceContext->CreateSolidColorBrush(
				D2D1::ColorF(RGB(32, 137, 77)),
				ptrBrush.GetAddressOf()
			);
		}

		return hr;
	}

	bool D2DApp::BeginDraw()
	{
		if (ptrD2DDeviceContext && ptrD2DRenderTarget && !bWindowMinimum && !bDestory)
		{
			ptrD2DDeviceContext->BeginDraw();
			return true;
		}
		return false;
	}

	void D2DApp::EndDraw()
	{
		ptrD2DDeviceContext->EndDraw();
		ptrSwapChain->Present(0, 0);
	}

	HRESULT D2DApp::OnCreate()
	{
		// Initialize device-indpendent resources, such as the Direct2D factory.
		HRESULT hr = CreateDeviceIndependentResources();

		// Initialize device-dependent resources.
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources();
		}

		if (SUCCEEDED(hr))
		{
			CreateWindowSizeDependentResources();
			hr = HandleCreateEvent(width, height) ? S_OK : S_FALSE;
		}

		if (SUCCEEDED(hr))
		{
			hr = CreateDrawTextResources();
		}

		return hr;
	}

	LRESULT D2DApp::OnActivate(const WPARAM& inFlags, const HWND& inHwnd)
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

	LRESULT D2DApp::OnKeyDown(const WPARAM& inKey)
	{
		// `inKey` is the virtual-key code of the nonsystem key. @see https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
		HandleKeyDownEvent(inKey);
		return FALSE;
	}

	LRESULT D2DApp::OnKeyUp(const WPARAM& inKey)
	{
		// `inKey`, the same as key down event.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keyup
		HandleKeyUpEvent(inKey);
		return FALSE;
	}

	LRESULT D2DApp::OnLeftMouseDown(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
	{
		// `inFlags`, the same as mouse move event.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-lbuttondown
		HandleLeftMouseDownEvent(inFlags, inX, inY);
		SetCapture(hWnd);
		PressingMouseNum++;
		return FALSE;
	}

	LRESULT D2DApp::OnLeftMouseUp(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
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

	LRESULT D2DApp::OnMiddleMouseDown(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
	{
		// `inFlags`, the same as mouse move event.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mbuttondown
		HandleMiddleMouseDownEvent(inFlags, inX, inY);
		SetCapture(hWnd);
		PressingMouseNum++;
		return FALSE;
	}

	LRESULT D2DApp::OnMiddleMouseUp(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
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

	LRESULT D2DApp::OnRightMouseDown(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
	{
		// `inFlags`, the same as mouse move event.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-rbuttondown
		HandleRightMouseDownEvent(inFlags, inX, inY);
		SetCapture(hWnd);
		PressingMouseNum++;
		return FALSE;
	}

	LRESULT D2DApp::OnRightMouseUp(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
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

	LRESULT D2DApp::OnMouseMove(const WPARAM& inFlags, const int32_t& inX, const int32_t& inY)
	{
		// `inFlags` indicates whether various virtual keys are down. This parameter can be one or more of the following values.
		//     MK_CONTROL(0x0008), MK_LBUTTON(0x0001), MK_MBUTTON(0x0010), MK_RBUTTON(0x0002),
		//     MK_SHIFT(0x0004), MK_XBUTTON1(0x0020), MK_XBUTTON2(0x0040)
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mousemove
		HandleMouseMoveEvent(inFlags, inX, inY);
		return FALSE;
	}

	LRESULT D2DApp::OnMouseWheel(const uint32_t& inFlags, const int16_t& inZDelta, const int32_t& inX, const int32_t& inY)
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

	LRESULT D2DApp::OnSizing(const WPARAM& inFlags)
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

	LRESULT D2DApp::OnSize(const WPARAM& inFlags, const uint32_t& inWidth, const uint32_t& inHeight)
	{
		// `inFlags` is the type of resizing requested. It can be one of the following values.
		//     SIZE_MAXHIDE(4), SIZE_MAXIMIZED(2), SIZE_MAXSHOW(3), SIZE_MINIMIZED(1), SIZE_RESTORED(0)
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-size
		width = inWidth;
		height = inHeight;
		rect.right = inWidth;
		rect.bottom = inHeight;

		if (inFlags == SIZE_MAXIMIZED)
		{
			CreateWindowSizeDependentResources();
			HandleResizeEvent(width, inHeight);
		}
		else if (inFlags == SIZE_RESTORED && bWindowMaximum)
		{
			CreateWindowSizeDependentResources();
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

	LRESULT D2DApp::OnExitSizeMove()
	{
		// Sent one time to a window, after it has exited the moving or sizing modal loop.
		// An application should return zero if it processes this message.
		// @see https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-exitsizemove
		if (bResizing)
		{
			bResizing = false;
			HandleResizingEvent(bResizing, width, height);
			CreateWindowSizeDependentResources();
			HandleResizeEvent(width, height);
		}
		return FALSE;
	}

	LRESULT D2DApp::OnDestroy()
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

	HRESULT D2DApp::Initialize(HINSTANCE hInstance, int32_t nCmdShow)
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
			wcex.lpfnWndProc = D2DApp::WndProc;
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

	int32_t D2DApp::Run()
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
			else if (BeginDraw())
			{
				frameCount++;
				timer.BeginFrame();

				EndDraw();

				timer.EndFrame();
			}
		}

		return (int32_t)msg.wParam;
	}

	LRESULT D2DApp::WndProc(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam)
	{
		static D2DApp* pThis = nullptr;

		switch (message)
		{
		furud_unlikely case WM_CREATE:
			pThis = static_cast<D2DApp*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
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