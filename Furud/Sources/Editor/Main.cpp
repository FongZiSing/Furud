//
// Main.cpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @Author FongZiSing
//
// The root entry of application.
//
#pragma once

#include "Engine.hpp"



INT APIENTRY wWinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR    lpCmdLine,
	_In_     INT     nCmdShow
)
{
	INT ReturnCode = -1;
	if (SUCCEEDED(::CoInitialize(nullptr)))
	{
		Furud::Engine FurudEngine;
		if (SUCCEEDED(FurudEngine.Initialize(hInstance, nCmdShow)))
		{
			ReturnCode = FurudEngine.Run();
		}
		::CoUninitialize();
	}
	return ReturnCode;
}