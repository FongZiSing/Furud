//
// Main.cpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @Author FongZiSing
//
// The root entry of application.
//
#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <objbase.h>

import Furud.Engine;



INT APIENTRY wWinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR    lpCmdLine,
	_In_     INT       nCmdShow
)
{
 	INT ReturnCode = -1;
	Furud::Engine FurudEngine;
	if (SUCCEEDED(FurudEngine.Initialize(hInstance, nCmdShow)))
	{
		ReturnCode = FurudEngine.Run();
	}
	return ReturnCode;
}