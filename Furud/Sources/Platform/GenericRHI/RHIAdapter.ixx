//
// RHIAdapter.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Adapter ( video card ).
//
module;

// Windows Header.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

// DirectX 12 header.
#include <d3d12.h>
#include <dxgi1_4.h>

// Smart Pointer Header.
#include <wrl/client.h>

// C++ Standard Library.
#include <stdint.h>
#include <format>
#include <vector>



export module Furud.Platform.RHI.Adapter;

import Furud.Platform.RHI.Verification;
using Microsoft::WRL::ComPtr;



export namespace Furud
{
	class RHIAdapter
	{
	private:
		DXGI_ADAPTER_DESC desc;
		UINT adapterIndex = -1;
		bool bActive = false;
		bool bVendorAMD : 1 { false };
		bool bVendorNVIDIA : 1 { false };
		bool bVendorIntel : 1 { false };
		bool bVendorWARP : 1 { false };

		D3D_FEATURE_LEVEL maxD3D12FeatureLevel;
		const wchar_t* strD3D12FeatureLevel;

		D3D_SHADER_MODEL maxD3D12ShaderModel;
		const wchar_t* strD3D12ShaderModel;


	private:
		RHIAdapter() = default;

		RHIAdapter(const DXGI_ADAPTER_DESC& inAdapterDesc, UINT inAdapterIndex)
			: desc(inAdapterDesc)
			, adapterIndex(inAdapterIndex)
		{}

		static bool D3D12CheckFeature_Internal(IDXGIAdapter* Adapter, D3D_FEATURE_LEVEL& outMaxFeatureLevel, D3D_SHADER_MODEL& outMaxShaderModel)
		{
			const D3D_FEATURE_LEVEL featureLevelsToCheck[] =
			{
				// Add new feature levels that the app supports here.
				D3D_FEATURE_LEVEL_12_1,
				D3D_FEATURE_LEVEL_12_0,
			};

			const D3D_SHADER_MODEL shaderModelsToCheck[] =
			{
				// Add new feature levels that the app supports here.
				D3D_SHADER_MODEL_6_5,
				D3D_SHADER_MODEL_6_4,
				D3D_SHADER_MODEL_6_3,
				D3D_SHADER_MODEL_6_2,
				D3D_SHADER_MODEL_6_1,
				D3D_SHADER_MODEL_6_0,
			};

			try
			{
				ComPtr<ID3D12Device> D3D12Device = nullptr;
				if (SUCCEEDED( D3D12CreateDevice(Adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(D3D12Device.GetAddressOf())) ))
				{
					// Determine the max feature level supported by the driver and hardware.
					D3D_FEATURE_LEVEL maxFeatureLevel = D3D_FEATURE_LEVEL_12_0;
					D3D12_FEATURE_DATA_FEATURE_LEVELS supportFeatureLevels {};
					supportFeatureLevels.pFeatureLevelsRequested = featureLevelsToCheck;
					supportFeatureLevels.NumFeatureLevels = _countof(featureLevelsToCheck);
					if (SUCCEEDED( D3D12Device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &supportFeatureLevels, sizeof(D3D12_FEATURE_DATA_FEATURE_LEVELS)) ))
					{
						maxFeatureLevel = supportFeatureLevels.MaxSupportedFeatureLevel;
					}

					// Determine the shader model.
					D3D_SHADER_MODEL maxShaderModel = D3D_SHADER_MODEL_5_1;
					D3D12_FEATURE_DATA_SHADER_MODEL supportShaderModel {};
					for (const D3D_SHADER_MODEL shaderModelToCheck : shaderModelsToCheck)
					{
						supportShaderModel.HighestShaderModel = shaderModelToCheck;
						if (SUCCEEDED( D3D12Device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &supportShaderModel, sizeof(D3D12_FEATURE_DATA_SHADER_MODEL)) ))
						{
							maxShaderModel = shaderModelToCheck;
							break;
						}
					}

					outMaxShaderModel = maxShaderModel;
					outMaxFeatureLevel = maxFeatureLevel;
					return true;
				}
			}
			catch (...) { /* TODO */ }

			return false;
		}

		static const wchar_t* ToString_Internal(const D3D_FEATURE_LEVEL& featureLevel)
		{
			switch (featureLevel)
			{
			case D3D_FEATURE_LEVEL_12_1: return L"D3D_FEATURE_LEVEL_12_1";
			case D3D_FEATURE_LEVEL_12_0: return L"D3D_FEATURE_LEVEL_12_0";
			default:
				return L"";
			}
		}

		static const wchar_t* ToString_Internal(const D3D_SHADER_MODEL& shaderModel)
		{
			switch (shaderModel)
			{
			case D3D_SHADER_MODEL_6_5: return L"D3D_SHADER_MODEL_6_5";
			case D3D_SHADER_MODEL_6_4: return L"D3D_SHADER_MODEL_6_4";
			case D3D_SHADER_MODEL_6_3: return L"D3D_SHADER_MODEL_6_3";
			case D3D_SHADER_MODEL_6_2: return L"D3D_SHADER_MODEL_6_2";
			case D3D_SHADER_MODEL_6_1: return L"D3D_SHADER_MODEL_6_1";
			case D3D_SHADER_MODEL_6_0: return L"D3D_SHADER_MODEL_6_0";
			default:
				return L"";
			}
		}

	public:
		static void EnumAdapters(ComPtr<IDXGIFactory> inDXGIFactory, std::vector<RHIAdapter>& outAllAdapter, ComPtr<IDXGIAdapter>& OutUseAdapter)
		{
			// Enumerates the adapters (video cards).
			outAllAdapter.clear();
	
			UINT index = 0;
			ComPtr<IDXGIAdapter> adapter;
			while (inDXGIFactory->EnumAdapters(index, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
			{
				DXGI_ADAPTER_DESC desc;
				VerifyD3D12Result( adapter->GetDesc(&desc) );

				D3D_FEATURE_LEVEL maxSupportedFeatureLevel = static_cast<D3D_FEATURE_LEVEL>(0);
				D3D_SHADER_MODEL maxSupportedShaderModel = D3D_SHADER_MODEL_5_1;

				if (D3D12CheckFeature_Internal(adapter.Get(), maxSupportedFeatureLevel, maxSupportedShaderModel))
				{
					// Enumerates the vendor.
					RHIAdapter rhiAdapter { desc, index };
					rhiAdapter.bVendorAMD    = (desc.VendorId == 0x1002);
					rhiAdapter.bVendorIntel  = (desc.VendorId == 0x8086);
					rhiAdapter.bVendorNVIDIA = (desc.VendorId == 0x10DE);
					rhiAdapter.bVendorWARP   = (desc.VendorId == 0x1414);
					rhiAdapter.maxD3D12FeatureLevel = maxSupportedFeatureLevel;
					rhiAdapter.strD3D12FeatureLevel = ToString_Internal(maxSupportedFeatureLevel);
					rhiAdapter.maxD3D12ShaderModel = maxSupportedShaderModel;
					rhiAdapter.strD3D12ShaderModel = ToString_Internal(maxSupportedShaderModel);
					
					// Skip WARP.
					if (!rhiAdapter.bVendorWARP && !OutUseAdapter)
					{
						OutUseAdapter = adapter;
					}

					outAllAdapter.push_back(rhiAdapter);
					++index;
					adapter->Release();
				}
			}

			for (auto& perAdapter : outAllAdapter)
			{
				std::wstring text = std::format(
					L"[Furud Engine] Adapter `{}`, Feature Level `{}`, Shader Model `{}`\n",
					perAdapter.desc.Description,
					perAdapter.strD3D12FeatureLevel,
					perAdapter.strD3D12ShaderModel);
				::OutputDebugStringW(text.c_str());
			}
		}
	};
}
