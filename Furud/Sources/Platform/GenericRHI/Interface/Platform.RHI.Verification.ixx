//
// Platform.RHI.Verification.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface - Verification utility.
//
module;

#include "../RHICommon.hpp"


export module Furud.Platform.RHI.Verification;

namespace Furud::Internal
{
	std::string GetD3D12ErrorInformation(HRESULT D3D12ErrorCode, ID3D12Device* D3D12Device);
}



export namespace Furud
{
	void VerifyD3D12Result(
		long D3D12ErrorCode,
		ID3D12Device* D3D12Device = nullptr,
		const char* message = "",
		const char* const fileName = __builtin_FILE(),
		const int codeLine = __builtin_LINE()
	)
	{
		if (SUCCEEDED(D3D12ErrorCode))
		{
			return;
		}

		std::string errorInformation = Internal::GetD3D12ErrorInformation(D3D12ErrorCode, D3D12Device);
		std::string errorString = std::format(
			"\'{}\'\n - at {}:{}\n - with error code: {}",
			message,
			fileName,
			codeLine,
			errorInformation
		);

		::MessageBoxA(nullptr, errorString.c_str(), "HR Failed!", MB_OK);

		// Windows debug break.
		{
			__nop(), __debugbreak();
		}
	}
}



module : private;

namespace Furud::Internal
{
	std::string GetD3D12ErrorInformation(HRESULT D3D12ErrorCode, ID3D12Device* D3D12Device)
	{
		// Enumerates the error code.
		std::string defaultD3D12Error;
		std::string_view resultOfD3D12Error;
		switch (D3D12ErrorCode)
		{
		case E_FAIL:                        resultOfD3D12Error = "E_FAIL"; break;
		case E_INVALIDARG:                  resultOfD3D12Error = "E_INVALIDARG"; break;
		case E_OUTOFMEMORY:                 resultOfD3D12Error = "E_OUTOFMEMORY"; break;
		case DXGI_ERROR_INVALID_CALL:       resultOfD3D12Error = "DXGI_ERROR_INVALID_CALL"; break;
		case DXGI_ERROR_WAS_STILL_DRAWING:  resultOfD3D12Error = "DXGI_ERROR_WAS_STILL_DRAWING"; break;
		case E_NOINTERFACE:                 resultOfD3D12Error = "E_NOINTERFACE"; break;
		case DXGI_ERROR_DEVICE_REMOVED:     resultOfD3D12Error = "DXGI_ERROR_DEVICE_REMOVED"; break;

		case DXGI_ERROR_UNSUPPORTED:                    resultOfD3D12Error = "DXGI_ERROR_UNSUPPORTED"; break;
		case DXGI_ERROR_NOT_CURRENT:                    resultOfD3D12Error = "DXGI_ERROR_NOT_CURRENT"; break;
		case DXGI_ERROR_MORE_DATA:                      resultOfD3D12Error = "DXGI_ERROR_MORE_DATA"; break;
		case DXGI_ERROR_MODE_CHANGE_IN_PROGRESS:        resultOfD3D12Error = "DXGI_ERROR_MODE_CHANGE_IN_PROGRESS"; break;
		case DXGI_ERROR_ALREADY_EXISTS:                 resultOfD3D12Error = "DXGI_ERROR_ALREADY_EXISTS"; break;
		case DXGI_ERROR_SESSION_DISCONNECTED:           resultOfD3D12Error = "DXGI_ERROR_SESSION_DISCONNECTED"; break;
		case DXGI_ERROR_ACCESS_DENIED:                  resultOfD3D12Error = "DXGI_ERROR_ACCESS_DENIED"; break;
		case DXGI_ERROR_NON_COMPOSITED_UI:              resultOfD3D12Error = "DXGI_ERROR_NON_COMPOSITED_UI"; break;
		case DXGI_ERROR_CACHE_FULL:                     resultOfD3D12Error = "DXGI_ERROR_CACHE_FULL"; break;
		case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:        resultOfD3D12Error = "DXGI_ERROR_NOT_CURRENTLY_AVAILABLE"; break;
		case DXGI_ERROR_CACHE_CORRUPT:                  resultOfD3D12Error = "DXGI_ERROR_CACHE_CORRUPT"; break;
		case DXGI_ERROR_WAIT_TIMEOUT:                   resultOfD3D12Error = "DXGI_ERROR_WAIT_TIMEOUT"; break;
		case DXGI_ERROR_FRAME_STATISTICS_DISJOINT:      resultOfD3D12Error = "DXGI_ERROR_FRAME_STATISTICS_DISJOINT"; break;
		case DXGI_ERROR_DYNAMIC_CODE_POLICY_VIOLATION:  resultOfD3D12Error = "DXGI_ERROR_DYNAMIC_CODE_POLICY_VIOLATION"; break;
		case DXGI_ERROR_REMOTE_OUTOFMEMORY:             resultOfD3D12Error = "DXGI_ERROR_REMOTE_OUTOFMEMORY"; break;
		case DXGI_ERROR_ACCESS_LOST:                    resultOfD3D12Error = "DXGI_ERROR_ACCESS_LOST"; break;
		default:
			defaultD3D12Error = std::format("0x{:08X}", D3D12ErrorCode);
			resultOfD3D12Error = defaultD3D12Error;
		}

		// Enumerates the reason of device removed.
		std::string defaultReasonOfDeviceRemoved;
		std::string_view reasonOfDeviceRemoved;
		if (D3D12ErrorCode == DXGI_ERROR_DEVICE_REMOVED && D3D12Device)
		{
			HRESULT nDeviceRemovedReason = D3D12Device->GetDeviceRemovedReason();
			switch (nDeviceRemovedReason)
			{
			case DXGI_ERROR_DEVICE_HUNG:            reasonOfDeviceRemoved = " with reason: DXGI_ERROR_DEVICE_HUNG"; break;
			case DXGI_ERROR_DEVICE_REMOVED:         reasonOfDeviceRemoved = " with reason: DXGI_ERROR_DEVICE_REMOVED"; break;
			case DXGI_ERROR_DEVICE_RESET:           reasonOfDeviceRemoved = " with reason: DXGI_ERROR_DEVICE_RESET"; break;
			case DXGI_ERROR_DRIVER_INTERNAL_ERROR:  reasonOfDeviceRemoved = " with reason: DXGI_ERROR_DRIVER_INTERNAL_ERROR"; break;
			case DXGI_ERROR_INVALID_CALL:           reasonOfDeviceRemoved = " with reason: DXGI_ERROR_INVALID_CALL"; break;
			default:
				defaultReasonOfDeviceRemoved = std::format(" with reason: 0x{:08X}", nDeviceRemovedReason);
				reasonOfDeviceRemoved = defaultReasonOfDeviceRemoved; break;
			}
		}

		std::string result;;
		result.reserve(resultOfD3D12Error.size() + reasonOfDeviceRemoved.size() + 1);
		result += resultOfD3D12Error;
		result += reasonOfDeviceRemoved;
		return result;
	}
}