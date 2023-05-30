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

	#ifndef furud_inline
	#define furud_inline [[msvc::forceinline]]
	#endif

	#ifndef furud_noinline
	#define furud_noinline [[msvc::noinline]]
	#endif

	#ifndef furud_intrinsic
	#define furud_intrinsic [[msvc::intrinsic]]
	#endif

	#ifndef furud_unused
	#define furud_unused [[maybe_unused]]
	#endif

	#ifndef furud_likely
	#define furud_likely [[likely]]
	#endif

	#ifndef furud_unlikely
	#define furud_unlikely [[unlikely]]
	#endif

	#ifndef furud_nodiscard
	#define furud_nodiscard [[nodiscard]]
	#endif

	#ifndef furud_deprecated
	#define furud_deprecated(message) [[deprecated(message)]]
	#endif

	#ifndef furud_restrict
	#define furud_restrict __restrict
	#endif

	#ifndef furud_interface
	#define furud_interface __declspec(novtable)
	#endif

	#ifndef furud_vectorapi
	#define furud_vectorapi __vectorcall
	#endif

	#ifndef furud_fastapi
	#define furud_fastapi __fastcall
	#endif

	#ifndef furud_softbreak
	#define furud_softbreak { *(volatile int*)0 = 0; }
	#endif

#endif



enum class ForceInitFlag : unsigned int {};
inline constexpr ForceInitFlag ForceInit{};
