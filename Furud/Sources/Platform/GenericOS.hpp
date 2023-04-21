//
// GenericOS.hpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// OS & compiler selection.
//
#pragma once



// Furud Engine is available only with C++20 or later.
#if defined(_MSVC_LANG) && !(_MSVC_LANG > 201703L) || !defined(__cplusplus)
	#error "[Furud] C++20 compiler required!"
#endif



// TODO
//   Furud Engine is available only with windows platform
//   and visual studio compiler now.
#if defined(WIN32) || defined(_WIN32) || defined(_WIN32_) || defined(WIN64) || defined(_WIN64) || defined(_WIN64_)
	#ifdef _MSC_VER
		#define FURUD_OS_WIN   1
		#define FURUD_OS_MAC   0
		#define FURUD_OS_LINUX 0
	#else
		#error "[Furud] unsupported operation system!"
	#endif
#endif
