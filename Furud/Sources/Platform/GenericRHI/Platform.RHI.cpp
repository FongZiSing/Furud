//
// RHI.cpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface.
//
module;

// Windows header.
#include "RHICommon.hpp"


module Furud.Platform.RHI;
import Furud.Platform.RHI.Verification;
import Furud.Platform.RHI.Device;
import Furud.Platform.RHI.Viewport;
import Furud.Platform.RHI.Adapter;
import Furud.Platform.RHI.Resource;
using Microsoft::WRL::ComPtr;
using namespace DirectX;


namespace Furud::inline TODO
{
	static UINT CalcConstantBufferByteSize(UINT byteSize)
	{
		// Constant buffers must be a multiple of the minimum hardware
		// allocation size (usually 256 bytes).  So round up to nearest
		// multiple of 256.  We do this by adding 255 and then masking off
		// the lower 2 bytes which store all bits < 256.
		// Example: Suppose byteSize = 300.
		// (300 + 255) & ~255
		// 555 & ~255
		// 0x022B & ~0x00ff
		// 0x022B & 0xff00
		// 0x0200
		// 512
		return (byteSize + 255) & ~255;
	}

	static XMFLOAT4X4 Identity4x4()
	{
		static XMFLOAT4X4 I(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return I;
	}

	template<typename T>
	class UploadBuffer
	{
	public:
		UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) :
			mIsConstantBuffer(isConstantBuffer)
		{
			mElementByteSize = sizeof(T);

			// Constant buffer elements need to be multiples of 256 bytes.
			// This is because the hardware can only view constant data 
			// at m*256 byte offsets and of n*256 byte lengths. 
			// typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
			// UINT64 OffsetInBytes; // multiple of 256
			// UINT   SizeInBytes;   // multiple of 256
			// } D3D12_CONSTANT_BUFFER_VIEW_DESC;
			if (isConstantBuffer)
				mElementByteSize = CalcConstantBufferByteSize(sizeof(T));

			D3D12_HEAP_PROPERTIES uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount);
			VerifyD3D12Result(device->CreateCommittedResource(
				&uploadHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&mUploadBuffer)));

			VerifyD3D12Result(mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));

			// We do not need to unmap until we are done with the resource.  However, we must not write to
			// the resource while it is in use by the GPU (so we must use synchronization techniques).
		}

		UploadBuffer(const UploadBuffer& rhs) = delete;
		UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
		~UploadBuffer()
		{
			if (mUploadBuffer != nullptr)
				mUploadBuffer->Unmap(0, nullptr);

			mMappedData = nullptr;
		}

		ID3D12Resource* Resource()const
		{
			return mUploadBuffer.Get();
		}

		void CopyData(int elementIndex, const T& data)
		{
			memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
		}

	private:
		ComPtr<ID3D12Resource> mUploadBuffer;
		BYTE* mMappedData = nullptr;

		UINT mElementByteSize = 0;
		bool mIsConstantBuffer = false;
	};

	ComPtr<ID3D12DescriptorHeap> CBVHeap;

	XMFLOAT4X4 mWorld = Identity4x4();
	XMFLOAT4X4 mView = Identity4x4();
	XMFLOAT4X4 mProj = Identity4x4();
	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

	struct ObjectConstants
	{
		XMFLOAT4X4 WorldViewProj = Identity4x4();
	};
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	ComPtr<ID3DBlob> CompileShader(
		const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target)
	{
		UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		HRESULT hr = S_OK;

		ComPtr<ID3DBlob> byteCode = nullptr;
		ComPtr<ID3DBlob> errors;
		hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

		if (errors != nullptr)
			OutputDebugStringA((char*)errors->GetBufferPointer());

		VerifyD3D12Result(hr);

		return byteCode;
	}

	ComPtr<ID3DBlob> mvsByteCode = nullptr;
	ComPtr<ID3DBlob> mpsByteCode = nullptr;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

	struct SubmeshGeometry
	{
		UINT IndexCount = 0;
		UINT StartIndexLocation = 0;
		INT BaseVertexLocation = 0;

		// Bounding box of the geometry defined by this submesh. 
		// This is used in later chapters of the book.
		DirectX::BoundingBox Bounds;
	};

	struct MeshGeometry
	{
		// Give it a name so we can look it up by name.
		std::string Name;

		// System memory copies.  Use Blobs because the vertex/index format can be generic.
		// It is up to the client to cast appropriately.  
		//Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr;
		//Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;

		// Data about the buffers.
		UINT VertexByteStride = 0;
		UINT VertexBufferByteSize = 0;
		DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
		UINT IndexBufferByteSize = 0;

		// A MeshGeometry may store multiple geometries in one vertex/index buffer.
		// Use this container to define the Submesh geometries so we can draw
		// the Submeshes individually.
		std::unordered_map<std::string, SubmeshGeometry> DrawArgs;

		D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
		{
			D3D12_VERTEX_BUFFER_VIEW vbv;
			vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
			vbv.StrideInBytes = VertexByteStride;
			vbv.SizeInBytes = VertexBufferByteSize;

			return vbv;
		}

		D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
		{
			D3D12_INDEX_BUFFER_VIEW ibv;
			ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
			ibv.Format = IndexFormat;
			ibv.SizeInBytes = IndexBufferByteSize;

			return ibv;
		}

		// We can free this memory after we finish upload to the GPU.
		void DisposeUploaders()
		{
			VertexBufferUploader = nullptr;
			IndexBufferUploader = nullptr;
		}
	};

	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;


	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* cmdList,
		const void* initData,
		UINT64 byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
	{
		ComPtr<ID3D12Resource> defaultBuffer;

		// Create the actual default buffer resource.
		D3D12_RESOURCE_DESC resouceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
		D3D12_HEAP_PROPERTIES defaultHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_HEAP_PROPERTIES uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		VerifyD3D12Result(device->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resouceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

		// In order to copy CPU memory data into our default buffer, we need to create
		// an intermediate upload heap. 
		VerifyD3D12Result(device->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resouceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(uploadBuffer.GetAddressOf())));


		// Describe the data we want to copy into the default buffer.
		D3D12_SUBRESOURCE_DATA subResourceData = {};
		subResourceData.pData = initData;
		subResourceData.RowPitch = byteSize;
		subResourceData.SlicePitch = subResourceData.RowPitch;

		// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
		// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
		// the intermediate upload heap data will be copied to mBuffer.
		D3D12_RESOURCE_BARRIER defaultResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		cmdList->ResourceBarrier(1, &defaultResourceBarrier);

		UpdateSubresources<1>(cmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);

		D3D12_RESOURCE_BARRIER uploadResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		cmdList->ResourceBarrier(1, &uploadResourceBarrier);

		// Note: uploadBuffer has to be kept alive after the above function calls because
		// the command list has not been executed yet that performs the actual copy.
		// The caller can Release the uploadBuffer after it knows the copy has been executed.


		return defaultBuffer;
	}

	ComPtr<ID3D12PipelineState> mPSO = nullptr;
}

namespace Furud::RHI
{
	bool bEnableDebugLayer = false;
	ComPtr<IDXGIFactory4> GRHIFactory;
	RHIAdapter GRHIAdapters;
	RHIDevice GRHIDevice;
	RHIViewport GRHIViewport;

	void Init()
	{
		// Enable the D3D12 debug layer.
#if defined(_DEBUG) || defined(DEBUG)
		ComPtr<ID3D12Debug> debugController;
		VerifyD3D12Result(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf())));
		debugController->EnableDebugLayer();
		bEnableDebugLayer = true;
#endif

		// Create DirectX Graphics Infrastructure Factory.
		VerifyD3D12Result(CreateDXGIFactory1(IID_PPV_ARGS(GRHIFactory.GetAddressOf())));

		// Enumerates adapters.
		GRHIAdapters.Initialize(GRHIFactory.Get());

		// Initialize devices.
		GRHIDevice.Initialize(GRHIFactory.Get(), GRHIAdapters.Get());

		// TODO
		GRHIDevice.ResetCommandList();

		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
		cbvHeapDesc.NumDescriptors = 1;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NodeMask = 0;
		VerifyD3D12Result(
			GRHIDevice.GetDevice()->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(CBVHeap.GetAddressOf())),
			GRHIDevice.GetDevice(),
			"D3D12Device->CreateDescriptorHeap()"
		);

		{
			mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(GRHIDevice.GetDevice(), 1, true);

			UINT objCBByteSize = CalcConstantBufferByteSize(sizeof(ObjectConstants));

			D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
			// Offset to the ith object constant buffer in the buffer.
			int boxCBufIndex = 0;
			cbAddress += boxCBufIndex * objCBByteSize;

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = cbAddress;
			cbvDesc.SizeInBytes = CalcConstantBufferByteSize(sizeof(ObjectConstants));

			GRHIDevice.GetDevice()->CreateConstantBufferView(
				&cbvDesc,
				CBVHeap->GetCPUDescriptorHandleForHeapStart());
		}

		{
			// Shader programs typically require resources as input (constant buffers,
			// textures, samplers).  The root signature defines the resources the shader
			// programs expect.  If we think of the shader programs as a function, and
			// the input resources as function parameters, then the root signature can be
			// thought of as defining the function signature.  

			// Root parameter can be a table, root descriptor or root constants.
			CD3DX12_ROOT_PARAMETER slotRootParameter[1];

			// Create a single descriptor table of CBVs.
			CD3DX12_DESCRIPTOR_RANGE cbvTable;
			cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
			slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

			// A root signature is an array of root parameters.
			CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
			ComPtr<ID3DBlob> serializedRootSig = nullptr;
			ComPtr<ID3DBlob> errorBlob = nullptr;
			HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
				serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

			if (errorBlob != nullptr)
			{
				::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			}
			VerifyD3D12Result(hr);

			VerifyD3D12Result(GRHIDevice.GetDevice()->CreateRootSignature(
				0,
				serializedRootSig->GetBufferPointer(),
				serializedRootSig->GetBufferSize(),
				IID_PPV_ARGS(&mRootSignature)));
		}

		{
			mvsByteCode = CompileShader(L"..\\Furud\\Sources\\Engine\\Renderer\\Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
			mpsByteCode = CompileShader(L"..\\Furud\\Sources\\Engine\\Renderer\\Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

			mInputLayout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			};
		}


		{
			std::array<Vertex, 8> vertices =
			{
				Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
				Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
				Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
				Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
				Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
				Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
				Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
				Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
			};

			std::array<std::uint16_t, 36> indices =
			{
				// front face
				0, 1, 2,
				0, 2, 3,

				// back face
				4, 6, 5,
				4, 7, 6,

				// left face
				4, 5, 1,
				4, 1, 0,

				// right face
				3, 2, 6,
				3, 6, 7,

				// top face
				1, 5, 6,
				1, 6, 2,

				// bottom face
				4, 0, 3,
				4, 3, 7
			};

			const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
			const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

			mBoxGeo = std::make_unique<MeshGeometry>();
			mBoxGeo->Name = "boxGeo";

			//VerifyD3D12Result(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
			//CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

			//VerifyD3D12Result(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
			//CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

			mBoxGeo->VertexBufferGPU = CreateDefaultBuffer(GRHIDevice.GetDevice(),
				GRHIDevice.GetCommandList(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

			mBoxGeo->IndexBufferGPU = CreateDefaultBuffer(GRHIDevice.GetDevice(),
				GRHIDevice.GetCommandList(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

			mBoxGeo->VertexByteStride = sizeof(Vertex);
			mBoxGeo->VertexBufferByteSize = vbByteSize;
			mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
			mBoxGeo->IndexBufferByteSize = ibByteSize;

			SubmeshGeometry submesh;
			submesh.IndexCount = (UINT)indices.size();
			submesh.StartIndexLocation = 0;
			submesh.BaseVertexLocation = 0;

			mBoxGeo->DrawArgs["box"] = submesh;
		}

		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
			ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
			psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
			psoDesc.pRootSignature = mRootSignature.Get();
			psoDesc.VS =
			{
				reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
				mvsByteCode->GetBufferSize()
			};
			psoDesc.PS =
			{
				reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
				mpsByteCode->GetBufferSize()
			};
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.NumRenderTargets = 1;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			psoDesc.SampleDesc.Count = 1;
			psoDesc.SampleDesc.Quality = 0;
			psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
			VerifyD3D12Result(GRHIDevice.GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
		}

		// Execute the initialization commands.
		GRHIDevice.ExecuteCommandList();

		// Wait until initialization is complete.
		GRHIDevice.FlushCommandQueue();
	}

	void CreateViewport(void* windowHandle, uint32_t clientWidth, uint32_t clientHeight)
	{
		GRHIViewport.Initialize(GRHIFactory.Get(), &GRHIDevice, (HWND)windowHandle, clientWidth, clientHeight);

		ResizeViewport(clientWidth, clientHeight);
	}

	void ResizeViewport(uint32_t clientWidth, uint32_t clientHeight)
	{
		// Flush before changing any resources.
		GRHIDevice.FlushCommandQueue();

		// Reusing the command list reuses memory.
		GRHIDevice.ResetCommandList();
		
		// Generate the resize commmand.
		GRHIViewport.Resize(&GRHIDevice, clientWidth, clientHeight);
		
		// Execute the resize commands.
		GRHIDevice.ExecuteCommandList();

		// Wait until resize is complete.
		GRHIDevice.FlushCommandQueue();

		// TODO
		// The window resized, so update the aspect ratio and recompute the projection matrix.
		XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, float(clientWidth) / clientHeight, 1.0f, 1000.0f);
		XMStoreFloat4x4(&mProj, P);
	}

	void DrawViewport()
	{
		// Reuse the memory associated with command recording.
		// We can only reset when the associated command lists have finished execution on the GPU.
		// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
		// Reusing the command list reuses memory.
		GRHIDevice.ResetCommandListAndAllocator(mPSO.Get());

		// Generate the draw commands.
		GRHIViewport.BeginDraw(GRHIDevice.GetCommandList());

		// Convert Spherical to Cartesian coordinates.
		float x = mRadius * sinf(mPhi) * cosf(mTheta);
		float z = mRadius * sinf(mPhi) * sinf(mTheta);
		float y = mRadius * cosf(mPhi);

		// Build the view matrix.
		XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
		XMVECTOR target = XMVectorZero();
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
		XMStoreFloat4x4(&mView, view);

		XMMATRIX world = XMLoadFloat4x4(&mWorld);
		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX worldViewProj = world * view * proj;

		// Update the constant buffer with the latest worldViewProj matrix.
		ObjectConstants objConstants;
		XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
		mObjectCB->CopyData(0, objConstants);

		auto mCommandList = GRHIDevice.GetCommandList();
		ID3D12DescriptorHeap* descriptorHeaps[] = { CBVHeap.Get() };
		mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

		mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

		D3D12_VERTEX_BUFFER_VIEW vView = mBoxGeo->VertexBufferView();
		mCommandList->IASetVertexBuffers(0, 1, &vView);

		D3D12_INDEX_BUFFER_VIEW iView = mBoxGeo->IndexBufferView();
		mCommandList->IASetIndexBuffer(&iView);
		
		mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mCommandList->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());

		mCommandList->DrawIndexedInstanced(
			mBoxGeo->DrawArgs["box"].IndexCount,
			1, 0, 0, 0);

		GRHIViewport.EndDraw(GRHIDevice.GetCommandList());

		// Execute the draw commands.
		GRHIDevice.ExecuteCommandList();

		// Swap the back and front buffers.
		GRHIViewport.Present();

		// Wait until frame commands are complete.  This waiting is inefficient and is
		// done for simplicity.  Later we will show how to organize our rendering code
		// so we do not have to wait per frame.
		GRHIDevice.FlushCommandQueue();

	}

	void FlushCommandQueue()
	{
		GRHIDevice.FlushCommandQueue();
	}

	RHIBufferRef CreateBuffer(const RHIBufferCreateInfo& info)
	{
		return new RHIBuffer({ GRHIDevice.GetDevice(), GRHIDevice.GetCommandQueue(),GRHIDevice.GetCommandList() }, info);
	}

}