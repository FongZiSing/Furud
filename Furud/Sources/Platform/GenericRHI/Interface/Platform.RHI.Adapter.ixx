//
// Platform.RHI.Adapter.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface - Adapter ( video card ).
//
module;

#include "../RHICommon.hpp"


export module Furud.Platform.RHI.Adapter;
import Furud.Platform.RHI.Verification;

export namespace Furud
{
	struct RHIAdapterDesc
	{
		DXGI_ADAPTER_DESC desc;
		UINT adapterIndex;
		bool bActive : 1;
		bool bVendorAMD : 1;
		bool bVendorNVIDIA : 1;
		bool bVendorIntel : 1;
		bool bVendorWARP : 1;

		D3D_FEATURE_LEVEL maxD3D12FeatureLevel;
		const wchar_t* strD3D12FeatureLevel;

		D3D_SHADER_MODEL maxD3D12ShaderModel;
		const wchar_t* strD3D12ShaderModel;
	};



	class RHIAdapter
	{
	private:
		std::vector<RHIAdapterDesc> AvailableAdapters;
		Microsoft::WRL::ComPtr<IDXGIAdapter> ChosenAdapter;


	private:
		/** Noncopyable. */
		RHIAdapter(const RHIAdapter&) = delete;

		/** Noncopyable. */
		RHIAdapter& operator = (const RHIAdapter&) = delete;

		/** Disable new. */
		void* operator new (size_t) = delete;


	private:
		static bool D3D12CheckFeature_Internal(IDXGIAdapter* Adapter, D3D_FEATURE_LEVEL& outMaxFeatureLevel, D3D_SHADER_MODEL& outMaxShaderModel);

		static const wchar_t* ToString_Internal(const D3D_FEATURE_LEVEL& featureLevel);

		static const wchar_t* ToString_Internal(const D3D_SHADER_MODEL& shaderModel);


	public:
		RHIAdapter() = default;

		IDXGIAdapter* Get() noexcept { return ChosenAdapter.Get(); }

		const IDXGIAdapter* Get() const noexcept { return ChosenAdapter.Get(); }

		void Initialize(IDXGIFactory* DXGIFactory);
	};
}



namespace Furud
{
	bool RHIAdapter::D3D12CheckFeature_Internal(IDXGIAdapter* Adapter, D3D_FEATURE_LEVEL& outMaxFeatureLevel, D3D_SHADER_MODEL& outMaxShaderModel)
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
			Microsoft::WRL::ComPtr<ID3D12Device> D3D12Device = nullptr;
			if (SUCCEEDED(D3D12CreateDevice(Adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(D3D12Device.GetAddressOf()))))
			{
				// Determine the max feature level supported by the driver and hardware.
				D3D_FEATURE_LEVEL maxFeatureLevel = D3D_FEATURE_LEVEL_12_0;
				D3D12_FEATURE_DATA_FEATURE_LEVELS supportFeatureLevels{};
				supportFeatureLevels.pFeatureLevelsRequested = featureLevelsToCheck;
				supportFeatureLevels.NumFeatureLevels = _countof(featureLevelsToCheck);
				if (SUCCEEDED(D3D12Device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &supportFeatureLevels, sizeof(D3D12_FEATURE_DATA_FEATURE_LEVELS))))
				{
					maxFeatureLevel = supportFeatureLevels.MaxSupportedFeatureLevel;
				}

				// Determine the shader model.
				D3D_SHADER_MODEL maxShaderModel = D3D_SHADER_MODEL_5_1;
				D3D12_FEATURE_DATA_SHADER_MODEL supportShaderModel{};
				for (const D3D_SHADER_MODEL shaderModelToCheck : shaderModelsToCheck)
				{
					supportShaderModel.HighestShaderModel = shaderModelToCheck;
					if (SUCCEEDED(D3D12Device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &supportShaderModel, sizeof(D3D12_FEATURE_DATA_SHADER_MODEL))))
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

	const wchar_t* RHIAdapter::ToString_Internal(const D3D_FEATURE_LEVEL& featureLevel)
	{
		switch (featureLevel)
		{
		case D3D_FEATURE_LEVEL_12_1: return L"D3D_FEATURE_LEVEL_12_1";
		case D3D_FEATURE_LEVEL_12_0: return L"D3D_FEATURE_LEVEL_12_0";
		default:
			return L"";
		}
	}

	const wchar_t* RHIAdapter::ToString_Internal(const D3D_SHADER_MODEL& shaderModel)
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

	void RHIAdapter::Initialize(IDXGIFactory* DXGIFactory)
	{
		AvailableAdapters.clear();
		ChosenAdapter.Reset();

		// Enumerates the adapters (video cards).
		UINT index = 0;
		Microsoft::WRL::ComPtr<IDXGIAdapter> tempAdapter;
		while (DXGIFactory->EnumAdapters(index, tempAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC desc;
			VerifyD3D12Result(tempAdapter->GetDesc(&desc));

			D3D_FEATURE_LEVEL maxSupportedFeatureLevel = static_cast<D3D_FEATURE_LEVEL>(0);
			D3D_SHADER_MODEL maxSupportedShaderModel = D3D_SHADER_MODEL_5_1;

			if (D3D12CheckFeature_Internal(tempAdapter.Get(), maxSupportedFeatureLevel, maxSupportedShaderModel))
			{
				// Enumerates the vendor.
				RHIAdapterDesc rhiAdapterDesc{};
				rhiAdapterDesc.desc = desc;
				rhiAdapterDesc.adapterIndex = index;
				rhiAdapterDesc.bVendorAMD = (desc.VendorId == 0x1002);
				rhiAdapterDesc.bVendorIntel = (desc.VendorId == 0x8086);
				rhiAdapterDesc.bVendorNVIDIA = (desc.VendorId == 0x10DE);
				rhiAdapterDesc.bVendorWARP = (desc.VendorId == 0x1414);
				rhiAdapterDesc.maxD3D12FeatureLevel = maxSupportedFeatureLevel;
				rhiAdapterDesc.strD3D12FeatureLevel = ToString_Internal(maxSupportedFeatureLevel);
				rhiAdapterDesc.maxD3D12ShaderModel = maxSupportedShaderModel;
				rhiAdapterDesc.strD3D12ShaderModel = ToString_Internal(maxSupportedShaderModel);

				// Skip WARP.
				if (!rhiAdapterDesc.bVendorWARP && !ChosenAdapter)
				{
					rhiAdapterDesc.bActive = true;
					ChosenAdapter = tempAdapter;
				}
				else
				{
					rhiAdapterDesc.bActive = false;
				}

				AvailableAdapters.push_back(rhiAdapterDesc);
				tempAdapter->Release();
				++index;
			}
		}

		for (auto& perAdapter : AvailableAdapters)
		{
			std::wstring text = std::format(
				L"[Furud Engine] Adapter `{}`, Feature Level `{}`, Shader Model `{}`, Use `{}`.\n",
				perAdapter.desc.Description,
				perAdapter.strD3D12FeatureLevel,
				perAdapter.strD3D12ShaderModel,
				(int)perAdapter.bActive);
			::OutputDebugStringW(text.c_str());
		}
	}
}