//
// Platform.RHI.Resource-Common.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface - Resource.
//
module;

#include "../RHICommon.hpp"


export module Furud.Platform.RHI.Resource:Common;
import :RefCounting;


export namespace Furud
{
	enum class ERHIResourceType : unsigned int
	{
		None = 0,
		Fence,
		Buffer,
		Texture,
		Num
	};



	class IRHIResource
	{
	protected:
		mutable IRHIThreadSafeCounter numRefs;

		std::string_view name;

		ERHIResourceType type;


	protected:
		struct RHIResourceContext
		{
			ID3D12Device* D3D12Device;
			ID3D12CommandQueue* D3D12CommandQueue;
			ID3D12GraphicsCommandList* D3D12CommandList;
		};


	public:
		explicit IRHIResource(ERHIResourceType inType) noexcept
			: numRefs(0)
			, name("None")
			, type(inType)
		{}

		virtual ~IRHIResource()
		{
			assert(numRefs.GetValue() == 0);
		}


	public:
		furud_inline ERHIResourceType GetType() const noexcept
		{
			return type;
		}

		furud_inline std::string_view GetName() const noexcept
		{
			return name;
		}


	public:
		furud_inline int32_t GetRefCount() const noexcept
		{
			return numRefs.GetValue();
		}

		furud_inline int32_t AddRef() const noexcept
		{
			return numRefs.Increment();
		}

		furud_inline int32_t Release() const noexcept
		{
			int32_t refs = numRefs.Decrement();
			if (refs == 0)
			{
				delete this;
			}
			return refs;
		}
	};
}