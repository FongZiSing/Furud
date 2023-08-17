//
// Platform.RHI.Resource-Buffer.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface - Resource Buffer.
//
module;

#include "../RHICommon.hpp"


export module Furud.Platform.RHI.Resource:Buffer;
import :Common;
import Furud.Platform.RHI.Verification;

export namespace Furud
{
	enum class ERHIBufferFlag : unsigned int
	{
		None = 0,
		StructuredBuffer,
		VertexBuffer,
		IndexBuffer,
		Num
	};



	struct RHIBufferCreateInfo
	{
		const char* name;

		const ERHIBufferFlag flag;

		const unsigned char* data;

		const unsigned int size;
	};



	class RHIBuffer : public IRHIResource
	{
	private:
		ERHIBufferFlag flag;

		unsigned int size;

		Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Resource> uploader = nullptr;


	protected:
		void InitRHI(RHIResourceContext&& context, RHIBufferCreateInfo const& info);


	public:
		RHIBuffer() = delete;

		RHIBuffer(RHIResourceContext&& context, RHIBufferCreateInfo const& info)
			: IRHIResource(ERHIResourceType::Buffer)
			, resource(nullptr)
			, uploader(nullptr)
		{
			InitRHI(std::forward<RHIResourceContext>(context), info);
		}

		ERHIBufferFlag GetFlag() const noexcept
		{
			return flag;
		}

		ID3D12Resource* GetResouce()
		{
			return resource.Get();
		}

		void ResetUploader()
		{
			uploader.Reset();
		}
	};
}



namespace Furud
{
	void RHIBuffer::InitRHI(RHIResourceContext&& context, RHIBufferCreateInfo const& info)
	{
		auto&& [D3D12Device, D3D12CommandQueue, D3D12CommandList] = context;
		name = info.name;
		flag = info.flag;
		size = info.size;
		resource.Reset();
		uploader.Reset();

		// Create the actual default buffer resource.
		D3D12_RESOURCE_DESC resouceDesc = CD3DX12_RESOURCE_DESC::Buffer(info.size);
		{
			D3D12_HEAP_PROPERTIES defaultHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			VerifyD3D12Result(D3D12Device->CreateCommittedResource(
				&defaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&resouceDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(resource.GetAddressOf())));
		}

		// In order to copy CPU memory data into our default buffer, we need to create
		// an intermediate upload heap. 
		{
			D3D12_HEAP_PROPERTIES uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			VerifyD3D12Result(D3D12Device->CreateCommittedResource(
				&uploadHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&resouceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(uploader.GetAddressOf())));
		}

		// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
		// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
		// the intermediate upload heap data will be copied to mBuffer.
		{
			D3D12_RESOURCE_BARRIER defaultResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
				resource.Get(),
				D3D12_RESOURCE_STATE_COMMON,
				D3D12_RESOURCE_STATE_COPY_DEST);
			D3D12CommandList->ResourceBarrier(1, &defaultResourceBarrier);

			// Describe the data we want to copy into the default buffer.
			D3D12_SUBRESOURCE_DATA subResourceData = {};
			subResourceData.pData = info.data;
			subResourceData.RowPitch = info.size;
			subResourceData.SlicePitch = subResourceData.RowPitch;
			UpdateSubresources<1>(D3D12CommandList, resource.Get(), uploader.Get(), 0, 0, 1, &subResourceData);

			D3D12_RESOURCE_BARRIER uploadResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
				resource.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_GENERIC_READ);
			D3D12CommandList->ResourceBarrier(1, &uploadResourceBarrier);
		}
	}
}