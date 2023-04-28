//
// Platform.Thread.Atomic.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Atomic operation interface.
//
module;

#include <Furud.hpp>
#include <atomic>



export module Furud.Platform.Thread.Atomic;

export namespace Furud
{
    enum class MemoryOrder : int
    {
        Relaxed = (int)std::memory_order_relaxed,
        Consume = (int)std::memory_order_consume,
        Acquire = (int)std::memory_order_acquire,
        Release = (int)std::memory_order_release,
        AcqRel  = (int)std::memory_order_acq_rel,
        SeqCst  = (int)std::memory_order_seq_cst,
    };



    template<typename V>
    struct Atomic : private std::atomic<V>
    {
        using TValue = V;


    private:
        using Super = std::atomic<V>;


    public:
        constexpr Atomic() : Super() {}

        using Super::Super;

        Atomic(const Atomic&) = delete;

        Atomic& operator = (const Atomic&) = delete;


    public:
        FURUD_INLINE TValue operator = (const TValue& value) volatile noexcept
        {
            return Super::operator = (value);
        }

        FURUD_INLINE TValue operator = (const TValue& value) noexcept
        {
            return Super::operator = (value);
        }

        FURUD_INLINE operator TValue() const volatile noexcept
        {
            return Super::load();
        }

        FURUD_INLINE operator TValue() const noexcept
        {
            return Super::load();
        }

        FURUD_INLINE void Set(const TValue& value) volatile noexcept
        {
            Super::store(value);
        }

        FURUD_INLINE void Set(const TValue& value, const MemoryOrder& order) volatile noexcept
        {
            Super::store(value, static_cast<std::memory_order>(order));
        }

        FURUD_NODISCARD FURUD_INLINE TValue Get() const volatile noexcept
        {
            return Super::load();
        }

        FURUD_NODISCARD FURUD_INLINE TValue Get(const MemoryOrder& order) const volatile noexcept
        {
            return Super::load(static_cast<std::memory_order>(order));
        }

        FURUD_INLINE TValue Exchange(const TValue& value) volatile noexcept
        {
            return Super::exchange(value);
        }

        FURUD_INLINE TValue Exchange(const TValue& value, const MemoryOrder& order) volatile noexcept
        {
            return Super::exchange(value, static_cast<std::memory_order>(order));
        }
    };
}