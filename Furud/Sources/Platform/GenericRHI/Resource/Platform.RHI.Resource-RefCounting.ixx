//
// Platform.RHI.Resource-RefCounting.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface - Reference Counting.
//
module;

#include <Furud.hpp>
#include <type_traits>


export module Furud.Platform.RHI.Resource:RefCounting;
import Furud.Platform.Thread.Atomics;


export namespace Furud
{
	template <typename TType>
	class TRHIRefCountPtr
	{
	private:
		TType* resource = nullptr;


	public:
		TRHIRefCountPtr() noexcept = default;

		~TRHIRefCountPtr()
		{
			if (resource)
			{
				resource->Release();
			}
		}

		TRHIRefCountPtr(TType* newResource)
		{
			resource = newResource;
			if (resource)
			{
				resource->AddRef();
			}
		}

		TRHIRefCountPtr(TRHIRefCountPtr const& rhs)
		{
			resource = rhs.resource;
			if (resource)
			{
				resource->AddRef();
			}
		}

		TRHIRefCountPtr(TRHIRefCountPtr&& rhs) noexcept
		{
			resource = rhs.resource;
			rhs.resource = nullptr;
		}

		template <typename OtherType>
		explicit TRHIRefCountPtr(TRHIRefCountPtr<OtherType> const& rhs)
		{
			static_assert(std::is_base_of_v<TType, OtherType>, "[Furud Engine] Unsupport type!");
			resource = static_cast<TType*>(rhs.resource);
			if (resource)
			{
				resource->AddRef();
			}
		}

		template <typename OtherType>
		explicit TRHIRefCountPtr(TRHIRefCountPtr<OtherType>&& rhs)
		{
			static_assert(std::is_base_of_v<TType, OtherType>, "[Furud Engine] Unsupport type!");
			resource = static_cast<TType*>(rhs.resource);
			rhs.resource = nullptr;
		}

		TRHIRefCountPtr& operator = (TRHIRefCountPtr const& rhs)
		{
			if (resource != rhs.resource)
			{
				TType* old = resource;
				resource = rhs.resource;
				if (resource)
				{
					resource->AddRef();
				}

				if (old)
				{
					old->Release();
				}
			}
			return *this;
		}

		TRHIRefCountPtr& operator = (TRHIRefCountPtr&& rhs)
		{
			TType* old = resource;
			resource = rhs.resource;
			rhs.resource = nullptr;
			if (old)
			{
				old->Release();
			}
			return *this;
		}

		template <typename OtherType>
		TRHIRefCountPtr& operator = (TRHIRefCountPtr<OtherType> const& rhs)
		{
			static_assert(std::is_base_of_v<TType, OtherType>, "[Furud Engine] Unsupport type!");
			if (resource != rhs.resource)
			{
				TType* old = resource;
				resource = static_cast<TType*>(rhs.resource);
				if (resource)
				{
					resource->AddRef();
				}

				if (old)
				{
					old->Release();
				}
			}
			return *this;
		}

		template <typename OtherType>
		TRHIRefCountPtr& operator = (TRHIRefCountPtr<OtherType>&& rhs)
		{
			static_assert(std::is_base_of_v<TType, OtherType>, "[Furud Engine] Unsupport type!");
			TType* old = resource;
			resource = static_cast<TType*>(rhs.resource);
			rhs.resource = nullptr;
			if (old)
			{
				old->Release();
			}
			return *this;
		}


	public:
		template <typename OtherType>
		furud_inline bool operator == (TRHIRefCountPtr<OtherType> const& rhs)
		{
			return resource == rhs.resource;
		}

		template <typename OtherType>
		furud_inline friend bool operator == (OtherType* lhs, TRHIRefCountPtr const& rhs)
		{
			return lhs == rhs.resource;
		}

		template <typename OtherType>
		furud_inline friend bool operator == (TRHIRefCountPtr const& lhs, OtherType* rhs)
		{
			return lhs.resource == rhs;
		}


	public:
		constexpr explicit operator bool() const noexcept { return resource != nullptr; }

		constexpr TType* operator -> () const noexcept { return resource; }

		constexpr TType& operator * () const noexcept { return *resource; }

		constexpr TType* Get() const noexcept { return resource; }

		constexpr TType& GetReference() const noexcept { return *resource; }
	};



	class IRHIThreadSafeCounter
	{
	private:
		/** Thread-safe counter. */
		volatile int counter;

		/** Hidden on purpose as usage wouldn't be thread safe. */
		void operator = (const IRHIThreadSafeCounter& other) {}


	public:
		IRHIThreadSafeCounter() noexcept 
			: counter(0)
		{}

		IRHIThreadSafeCounter(const IRHIThreadSafeCounter& rhs) noexcept
		{
			counter = rhs.GetValue();
		}

		IRHIThreadSafeCounter(int value) noexcept
		{
			counter = value;
		}

		furud_inline int Increment() noexcept
		{
			return IAtomic32::Increment(&counter);
		}

		furud_inline int Decrement() noexcept
		{
			return IAtomic32::Decrement(&counter);
		}

		furud_inline int GetValue() const noexcept
		{
			return IAtomic32::Read(&counter);
		}
	};
}