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

// C++ Standard Library.
#include <stdint.h>
#include <string>
#include <format>

// Furud headers.
#include <Furud.hpp>



module Furud.App;

import Furud.Platform.API.FrameTimer;
import Furud.Platform.RHI;

namespace Furud
{
	D3DApp::D3DApp()
	{
	}

	D3DApp::~D3DApp()
	{
		RHI::FlushCommandQueue();
	}

	void D3DApp::Draw(const float deltaTime)
	{
		RHI::DrawViewport();
	}

	HRESULT D3DApp::OnCreate()
	{
		RHI::Init();
		RECT rect = { 0, 0, (LONG)width, (LONG)height };
		::GetClientRect(hWnd, &rect);
		RHI::CreateViewport(hWnd, rect.right - rect.left, rect.bottom - rect.top);

		// Dispatch create event.
		return HandleCreateEvent(width, height) ? S_OK : UI_E_CREATE_FAILED;
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
			RHI::ResizeViewport(width, height);
			HandleResizeEvent(width, inHeight);
		}
		else if (inFlags == SIZE_RESTORED && bWindowMaximum)
		{
			RHI::ResizeViewport(width, height);
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
			RHI::ResizeViewport(width, height);
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