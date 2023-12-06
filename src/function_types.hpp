#pragma once
#include "common.hpp"
#include "class/unreal_engine_class.hpp"

namespace big::functions
{
	using process_event_t = void(*)(UObject* _this, UFunction* func, void* params);
	using create_d3d11_device_and_swapchain_t = long(__stdcall*)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
}
