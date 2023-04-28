//
// Platform.Thread.Atomics.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Atomic operation interface.
//
module;

#include <Furud.hpp>
#include <intrin.h>
#include <concepts>



export module Furud.Platform.Thread.Atomics;

namespace Furud
{
	/**
	 * @brief    Atomic operation interface.
	 * @tparam   T  -  number type.
	 * @details  原子操作
	 */
	template <typename T>
	struct TAtomics
	{
		/**
		 * @brief    Does an interlocked compare and exchange.
		 *           Stores the `exchange` in the `*dst` if `*dst` is equals to `comparand`.
		 * @param    dst        -  Pointer to the destination value.
		 * @param    exchange   -  Exchange value.
		 * @param    comparand  -  Value to compare to destination.
		 * @returns  the old destination value.
		 * @details  原子测试。
		 */
		FURUD_INLINE static T CompareAndExchange(volatile T* dst, const T& exchange, const T& comparand)
		{
			// see https://learn.microsoft.com/en-us/cpp/intrinsics/interlockedcompareexchange-intrinsic-functions
			if constexpr (std::same_as<T, char>)
			{
				return ::_InterlockedCompareExchange8(dst, exchange, comparand);
			}
			else if constexpr (std::same_as<T, short>)
			{
				return ::_InterlockedCompareExchange16(dst, exchange, comparand);
			}
			else if constexpr (std::same_as<T, int> || std::same_as<T, long>)
			{
				return ::_InterlockedCompareExchange((volatile long*)dst, (long)exchange, (long)comparand);
			}
			else if constexpr (std::same_as<T, long long>)
			{
				return ::_InterlockedCompareExchange64(dst, exchange, comparand);
			}
			// see https://learn.microsoft.com/en-us/cpp/intrinsics/interlockedcompareexchangepointer-intrinsic-functions
			else if constexpr (std::same_as<T, void*>)
			{
				return ::_InterlockedCompareExchangePointer(dst, exchange, comparand);
			}
			else
			{
				static_assert(!sizeof(T), "[Furud] Unsupported type!");
			}
		}


		/**
		 * @brief    Does an interlocked read.
		 * @details  原子读。
		 */
		FURUD_INLINE static T Read(volatile const T* dst)
		{
			return CompareAndExchange(const_cast<volatile T*>(dst), T(0), T(0));
		}


		/**
		 * @brief    Does an interlocked write.
		 * @returns  The old destination value.
		 * @details  原子写。
		 */
		FURUD_INLINE static T Write(volatile T* dst, const T& value)
		{
			// see https://learn.microsoft.com/en-us/cpp/intrinsics/interlockedexchange-intrinsic-functions
			if constexpr (std::same_as<T, char>)
			{
				return ::_InterlockedExchange8(dst, value);
			}
			else if constexpr (std::same_as<T, short>)
			{
				return ::_InterlockedExchange16(dst, value);
			}
			else if constexpr (std::same_as<T, int> || std::same_as<T, long>)
			{
				return ::_InterlockedExchange((volatile long*)dst, (long)value);
			}
			else if constexpr (std::same_as<T, long long>)
			{
				return ::_InterlockedExchange64(dst, value);
			}
			// see https://learn.microsoft.com/en-us/cpp/intrinsics/interlockedexchangepointer-intrinsic-functions
			else if constexpr (std::same_as<T, void*>)
			{
				return ::_InterlockedExchangePointer(dst, value);
			}
			else
			{
				static_assert(!sizeof(T), "[Furud] Unsupported type!");
			}
		}


		/**
		 * @brief    Does an interlocked increment.
		 * @returns  destination value + 1.
		 * @details  原子自增。
		 */
		FURUD_INLINE static T Increment(volatile T* dst)
		{
			// see https://learn.microsoft.com/en-us/cpp/intrinsics/interlockedexchangeadd-intrinsic-functions
			if constexpr (std::same_as<T, char>)
			{
				return ::_InterlockedExchangeAdd8(dst, 1) + 1;
			}
			// see https://learn.microsoft.com/en-us/cpp/intrinsics/interlockedincrement-intrinsic-functions
			else if constexpr (std::same_as<T, short>)
			{
				return ::_InterlockedIncrement16(dst);
			}
			else if constexpr (std::same_as<T, int> || std::same_as<T, long>)
			{
				return ::_InterlockedIncrement((volatile long*)dst);
			}
			else if constexpr (std::same_as<T, long long>)
			{
				return ::_InterlockedIncrement64(dst);
			}
			// see https://learn.microsoft.com/en-us/cpp/intrinsics/interlockedexchangepointer-intrinsic-functions
			else if constexpr (std::same_as<T, void*>)
			{
				return ::_InterlockedExchangePointer(dst, *dst + 1) + 1;
			}
			else
			{
				static_assert(!sizeof(T), "[Furud] Unsupported type!");
			}
		}


		/**
		 * @brief    Does an interlocked decrement.
		 * @returns  destination value - 1.
		 * @details  原子自减。
		 */
		FURUD_INLINE static T Decrement(volatile T* dst)
		{
			// see https://learn.microsoft.com/en-us/cpp/intrinsics/interlockedexchangeadd-intrinsic-functions
			if constexpr (std::same_as<T, char>)
			{
				return ::_InterlockedExchangeAdd8(dst, -1) - 1;
			}
			// see https://learn.microsoft.com/en-us/cpp/intrinsics/interlockeddecrement-intrinsic-functions
			else if constexpr (std::same_as<T, short>)
			{
				return ::_InterlockedDecrement16(dst);
			}
			else if constexpr (std::same_as<T, int> || std::same_as<T, long>)
			{
				return ::_InterlockedDecrement((volatile long*)dst);
			}
			else if constexpr (std::same_as<T, long long>)
			{
				return ::_InterlockedDecrement64(dst);
			}
			// see https://learn.microsoft.com/en-us/cpp/intrinsics/interlockedexchangepointer-intrinsic-functions
			else if constexpr (std::same_as<T, void*>)
			{
				return ::_InterlockedExchangePointer(dst, *dst - 1) - 1;
			}
			else
			{
				static_assert(!sizeof(T), "[Furud] Unsupported type!");
			}
		}
	};
}



export namespace Furud
{
	using IAtomic8   = TAtomics<char>;
	using IAtomic16  = TAtomics<short>;
	using IAtomic32  = TAtomics<int>;
	using IAtomic64  = TAtomics<long long>;
	using IAtomicPtr = TAtomics<void*>;
}