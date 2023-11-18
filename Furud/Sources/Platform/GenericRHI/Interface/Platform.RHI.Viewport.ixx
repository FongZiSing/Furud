//
// Platform.RHI.Viewport.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface - Viewport ( client area of application ).
//
module;

#include "../RHICommon.hpp"


export module Furud.Platform.RHI.Viewport;
import Furud.Platform.RHI.Verification;
import Furud.Platform.RHI.Device;

export namespace Furud
{
	class RHIViewport
	{
	private:
		D3D12_VIEWPORT screenViewport;
		D3D12_RECT scissorRect;

		static const UINT numSwapChainBuffer = 2;
		INT swapChainIndex = 0;
		Microsoft::WRL::ComPtr<IDXGISwapChain> DXGISwapChain;

		// TODO: Descriptor heap manager.
		UINT RTVDescriptorSize = 0;
		UINT DSVDescriptorSize = 0;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DSVHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> renderTargetBuffers[numSwapChainBuffer];
		Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilBuffer;


	public:
		void Initialize(IDXGIFactory* DXGIFactory, RHIDevice* Device, HWND windowHandle, UINT clientWidth, UINT clientHeight);

		void Resize(RHIDevice* Device, UINT clientWidth, UINT clientHeight);

		void BeginDraw(ID3D12GraphicsCommandList* commandList);

		void EndDraw(ID3D12GraphicsCommandList* commandList);

		void Present();
	};
}



namespace Furud
{
	void RHIViewport::Initialize(IDXGIFactory* DXGIFactory, RHIDevice* Device, HWND windowHandle, UINT clientWidth, UINT clientHeight)
	{
		// Release the previous swapchain we will be recreating.
		DXGISwapChain.Reset();

		// Create the swap chain.
		{
			DXGI_SWAP_CHAIN_DESC swapChainDesc{};
			swapChainDesc.BufferDesc.Width = clientWidth;
			swapChainDesc.BufferDesc.Height = clientHeight;
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 120;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
			swapChainDesc.BufferCount = numSwapChainBuffer;
			swapChainDesc.OutputWindow = windowHandle;
			swapChainDesc.Windowed = true;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			VerifyD3D12Result(
				DXGIFactory->CreateSwapChain(
					Device->GetCommandQueue(),
					&swapChainDesc,
					DXGISwapChain.GetAddressOf())
			);
		}

		// Careate RTV, RTV.
		{
			// Descriptor Heap of render target view.
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
			rtvHeapDesc.NumDescriptors = numSwapChainBuffer;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			rtvHeapDesc.NodeMask = 0;
			VerifyD3D12Result(
				Device->GetDevice()->CreateDescriptorHeap(
					&rtvHeapDesc,
					IID_PPV_ARGS(RTVHeap.GetAddressOf()))
			);
			RTVDescriptorSize = Device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			assert(RTVDescriptorSize > 0 && "Unexpected descriptor size!");

			// Descriptor Heap of depth stencil view.
			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			dsvHeapDesc.NodeMask = 0;
			VerifyD3D12Result(
				Device->GetDevice()->CreateDescriptorHeap(
					&dsvHeapDesc,
					IID_PPV_ARGS(DSVHeap.GetAddressOf()))
			);
			DSVDescriptorSize = Device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			assert(DSVDescriptorSize > 0 && "Unexpected descriptor size!");
		}
	}

	void RHIViewport::Resize(RHIDevice* Device, UINT clientWidth, UINT clientHeight)
	{
		// Update the viewport transform to cover the client area.
		{
			screenViewport.TopLeftX = 0;
			screenViewport.TopLeftY = 0;
			screenViewport.Width = static_cast<float>(clientWidth);
			screenViewport.Height = static_cast<float>(clientHeight);
			screenViewport.MinDepth = 0.0f;
			screenViewport.MaxDepth = 1.0f;
			scissorRect = { 0, 0, static_cast<LONG>(clientWidth), static_cast<LONG>(clientHeight) };
		}

		// Release the previous resources we will be recreating.
		{
			for (UINT i = 0; i < numSwapChainBuffer; ++i)
			{
				renderTargetBuffers[i].Reset();
			}
			depthStencilBuffer.Reset();
			swapChainIndex = 0;
		}

		// Create the render target view.
		{
			// Resize the swap chain.
			VerifyD3D12Result(
				DXGISwapChain->ResizeBuffers(
					numSwapChainBuffer,
					clientWidth,
					clientHeight,
					DXGI_FORMAT_R8G8B8A8_UNORM,
					DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)
			);

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
			for (UINT i = 0; i < numSwapChainBuffer; ++i)
			{
				VerifyD3D12Result(DXGISwapChain->GetBuffer(i, IID_PPV_ARGS(renderTargetBuffers[i].GetAddressOf())));
				Device->GetDevice()->CreateRenderTargetView(renderTargetBuffers[i].Get(), nullptr, rtvHeapHandle);
				rtvHeapHandle.Offset(1, RTVDescriptorSize);
			}
		}

		// Create the depth/stencil buffer and view.
		{
			// Allocates the resource.
			D3D12_RESOURCE_DESC depthStencilDesc;
			depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			depthStencilDesc.Alignment = 0;
			depthStencilDesc.Width = clientWidth;
			depthStencilDesc.Height = clientHeight;
			depthStencilDesc.DepthOrArraySize = 1;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
			depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

			D3D12_CLEAR_VALUE clearValue;
			clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			clearValue.DepthStencil.Depth = 1.0f;
			clearValue.DepthStencil.Stencil = 0;

			D3D12_HEAP_PROPERTIES dsvHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			VerifyD3D12Result(
				Device->GetDevice()->CreateCommittedResource(
					&dsvHeapProperties,
					D3D12_HEAP_FLAG_NONE,
					&depthStencilDesc,
					D3D12_RESOURCE_STATE_COMMON,
					&clearValue,
					IID_PPV_ARGS(depthStencilBuffer.GetAddressOf()))
			);

			// Create the resource view.
			D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
			depthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;
			depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilViewDesc.Texture2D.MipSlice = 0;
			Device->GetDevice()->CreateDepthStencilView(
				depthStencilBuffer.Get(),
				&depthStencilViewDesc,
				DSVHeap->GetCPUDescriptorHandleForHeapStart());

			// Transition the resource from its initial state to be used as a depth buffer.
			D3D12_RESOURCE_BARRIER dsvBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
				depthStencilBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON,
				D3D12_RESOURCE_STATE_DEPTH_WRITE);
			Device->GetCommandList()->ResourceBarrier(1, &dsvBarrier);
		}
	}

	void RHIViewport::BeginDraw(ID3D12GraphicsCommandList* commandList)
	{
		commandList->RSSetViewports(1, &screenViewport);
		commandList->RSSetScissorRects(1, &scissorRect);

		CD3DX12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView
		{
			RTVHeap->GetCPUDescriptorHandleForHeapStart(),
			swapChainIndex,
			RTVDescriptorSize
		};

		CD3DX12_CPU_DESCRIPTOR_HANDLE DepthStencilView
		{
			DSVHeap->GetCPUDescriptorHandleForHeapStart()
		};

		// Indicate a state transition on the resource usage.
		D3D12_RESOURCE_BARRIER beginSwapChainBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			renderTargetBuffers[swapChainIndex].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		commandList->ResourceBarrier(1, &beginSwapChainBarrier);

		// Clear the back buffer and depth buffer.
		commandList->ClearRenderTargetView(
			CurrentBackBufferView,
			DirectX::Colors::LightSteelBlue,
			0,
			nullptr);

		commandList->ClearDepthStencilView(
			DSVHeap->GetCPUDescriptorHandleForHeapStart(),
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
			1.0f,
			0,
			0,
			nullptr);

		// Specify the buffers we are going to render to.
		commandList->OMSetRenderTargets(
			1,
			&CurrentBackBufferView,
			true,
			&DepthStencilView);
	}

	void RHIViewport::EndDraw(ID3D12GraphicsCommandList* commandList)
	{
		// Indicate a state transition on the resource usage.
		D3D12_RESOURCE_BARRIER endSwapChainBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			renderTargetBuffers[swapChainIndex].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT);
		commandList->ResourceBarrier(1, &endSwapChainBarrier);
	}

	void RHIViewport::Present()
	{
		// Swap the back and front buffers.
		DXGISwapChain->Present(0, 0);
		swapChainIndex = (swapChainIndex + 1) % numSwapChainBuffer;
	}
}