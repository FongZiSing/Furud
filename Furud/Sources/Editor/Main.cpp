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
import Furud.Platform.API.CharArray;
import Furud.Platform.Thread;
import Furud.Platform.Thread.Atomics;
import Furud.Platform.Thread.Parallel;
import Furud.Platform.Memory;
import Furud.Platform.Memory.FileStream;
import Furud.Platform.SIMD;
import Furud.Platform.Math;
import Furud.Core.Matrix;
import Furud.Platform.RHI.Viewport;

INT APIENTRY wWinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR    lpCmdLine,
	_In_     INT     nCmdShow
)
{
	/*
	Furud::Vector4f mmm { 123, 123, 123, 1 };
	Furud::Matrix44f d1 = Furud::Matrix44f::IDENTITY;
	Furud::Matrix44f d2 = 4;
	d1 += d2;
	mmm = d1.TransformPosition4(mmm);
	Furud::MemoryLeakValidation();

	Furud::AnsicharArrayView s = "111.2f";
	constexpr Furud::AnsicharArrayView t = " -12134 dada 1223 ";
	constexpr Furud::AnsicharArrayView::Pair i = t.Split(' ');

	const Furud::AnsicharArray b = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklumopqrstuvwxyz_1234567890";
	Furud::AnsicharArray c = "dadad";
	Furud::Thread thread;
	thread.Init({ .threadName = "hello world" });
	c = b.Clone();
	//b.Reserve(b.Size() + 100);
	//b.Push('.');
	//b.ToUpper();
	//b.ToLower();
	Furud::AnsicharArrayView a = b.Sub(b.Size() - 1, 5);
	int n = 0;

	Furud::IParallel::For(1024, [&n](int i) {
		Furud::IAtomic32::Increment(&n);
	});

	char buffer[1024];
	Furud::AnsicharStream Ips;
	if (Ips.Open(L"D:/1.txt"))
	{
		Furud::AnsicharArray m;
		Ips.GetLine(m);
		Ips.GetLine(m);
		Ips.GetLine(m);
		Ips.GetLine(m);
	}

	Furud::Vec8f mm = Furud::Vec8f::SIGN_BIT;
	float u = Furud::IMath::Floor(1200.4f);
	*/

 	INT ReturnCode = -1;
	//if (SUCCEEDED(::CoInitialize(nullptr)))
	{
		Furud::Engine FurudEngine;
		if (SUCCEEDED(FurudEngine.Initialize(hInstance, nCmdShow)))
		{
			ReturnCode = FurudEngine.Run();
		}
		//::CoUninitialize();
	}
	return ReturnCode;
}