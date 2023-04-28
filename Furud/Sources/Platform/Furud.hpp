//
// Furud.hpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Useful macro definitions.
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



#if FURUD_OS_WIN

	#ifndef FURUD_INLINE
	#define FURUD_INLINE [[msvc::forceinline]]
	#endif

	#ifndef FURUD_NOINLINE
	#define FURUD_NOINLINE [[msvc::noinline]]
	#endif

	#ifndef FURUD_INTRINSIC
	#define FURUD_INTRINSIC [[msvc::intrinsic]]
	#endif

	#ifndef FURUD_UNUSED
	#define FURUD_UNUSED [[maybe_unused]]
	#endif

	#ifndef FURUD_LIKELY
	#define FURUD_LIKELY [[likely]]
	#endif

	#ifndef FURUD_UNLIKELY
	#define FURUD_UNLIKELY [[unlikely]]
	#endif

	#ifndef FURUD_NODISCARD
	#define FURUD_NODISCARD [[nodiscard]]
	#endif

	#ifndef FURUD_DEPRECATED
	#define FURUD_DEPRECATED(message) [[deprecated(message)]]
	#endif

	#ifndef FURUD_RESTRICT
	#define FURUD_RESTRICT __restrict
	#endif

	#ifndef FURUD_INTERFACE
	#define FURUD_INTERFACE __declspec(novtable)
	#endif

	#ifndef FURUD_VECTORAPI
	#define FURUD_VECTORAPI __vectorcall
	#endif

	#ifndef FURUD_FASTAPI
	#define FURUD_FASTAPI __fastcall
	#endif

	#ifndef FURUD_SOFTBREAK
	#define FURUD_SOFTBREAK { *(volatile int*)0 = 0; }
	#endif

#endif



enum class ForceInitFlag : unsigned int {};
inline constexpr ForceInitFlag ForceInit{};
