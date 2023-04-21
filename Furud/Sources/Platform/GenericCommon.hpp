//
// GenericCommon.hpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Useful macro definitions.
//
#pragma once



#ifndef force_inline
#define force_inline __forceinline
#endif

#ifndef force_noinline
#define force_noinline __declspec(noinline)
#endif

#ifndef inline_var
#define inline_var __declspec(selectany)
#endif

#ifndef restrict_var
#define restrict_var __restrict
#endif

#ifndef restrict_func
#define restrict_func __declspec(restrict)
#endif

#ifndef warn_nodiscard
#define warn_nodiscard [[nodiscard]]
#endif

#ifndef warn_deprecated
#define warn_deprecated(message) [[deprecated(message)]]
#pragma warning(default: 4996)
#endif

#ifndef force_softbreak
#define force_softbreak *(volatile int*)0 = 0
#endif

#ifndef as_interface
#define as_interface __declspec(novtable)
#endif

#ifndef as_allocator
#define as_allocator __declspec(allocator)
#endif

#ifndef branch_likely
#define branch_likely [[likely]]
#endif

#ifndef branch_unlikely
#define branch_unlikely [[unlikely]]
#endif

#ifndef vector_call
#define vector_call __vectorcall
#endif

#ifndef fast_call
#define fast_call __fastcall
#endif

#ifndef in_ptr
#define in_ptr const*const
#endif

#ifndef out_ptr
#define out_ptr *
#endif

#ifndef in_ref
#define in_ref const&
#endif

#ifndef out_ref
#define out_ref &
#endif



using int8  = char;
using int16 = short;
using int32 = int;
using int64 = long long;

using uint8  = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

using wchar = wchar_t;
static_assert(sizeof(wchar) == sizeof(uint16));



#define FURUD_API