#include "common.hpp"
#include "logger.hpp"
#include "pointers.hpp"
#include "memory/all.hpp"

namespace big
{
	pointers::pointers()
	{
		memory::pattern_batch main_batch;

		for (int i = 0; i <= 5 && !swapchain_found; i++)
		{
			swapchain_found = this->get_swapchain();
			std::this_thread::sleep_for(1000ms);
		}
		
		main_batch.add("Engine", "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 85 ? 74 ? 33 DB", [this](memory::handle ptr)
		{
			m_engine = ptr.add(3).rip().as<decltype(m_engine)>();
		});

		main_batch.add("Process Event", "E8 ? ? ? ? 48 8B 74 24 ? 48 8B 5C 24 ? 48 8B 6C 24 ? 48 83 C4 20 5F C3 40 57", [this](memory::handle ptr)
		{
			m_process_event = ptr.add(1).rip().as<decltype(m_process_event)>();
		});

		main_batch.add("GUObjectArray", "48 8B 05 ? ? ? ? C1 F9 10 48 63 C9 48 8B 14 C8 4B 8D 0C 40 4C 8D 04 CA EB 03", [this](memory::handle ptr)
		{
			m_object_array = ptr.add(3).rip().as<decltype(m_object_array)>();
		});

		main_batch.add("FName Pool", "48 8D 0D ? ? ? ? E8 ? ? ? ? C6 05 ? ? ? ? ? 0F 10 03 4C 8D 44 24 ? 48 8B C8 48 8D 54 24 ? 0F 29 44 24", [this](memory::handle ptr)
		{
			m_name = ptr.add(3).rip().as<decltype(m_name)>();
		});

		main_batch.add("Battle Points Hanlder", "48 89 ? ? ? 57 48 83 EC ? 8B FA 48 8B ? E8 ? ? ? ? 3C ? 75 ? 8B 93", [this](memory::handle ptr)
		{
			m_battle_points_handle = ptr.as<decltype(m_battle_points_handle)>();
		});

		main_batch.add("Credits Handler", "48 89 ? ? ? 48 89 ? ? ? 48 89 ? ? ? 57 48 83 EC ? 4C 8B ? ? ? ? ? 33 F6", [this](memory::handle ptr)
		{
			m_credits_handle = ptr.as<decltype(m_credits_handle)>();
		});

		main_batch.add("Items Handler", "45 8D ? ? 45 89 ? ? 45 3B", [this](memory::handle ptr)
		{
			m_items_handle = ptr.as<decltype(m_items_handle)>();
		});
		
		main_batch.run(memory::module(nullptr));

		//ScarletNexus.exe+4E91CA0 / 7FF7CA081CA0 UObject
		//ScarletNexus.exe+4E65E00 / 7FF7CA055E00 FNamePool

		this->m_hwnd = FindWindowW(L"UnrealWindow", L"ScarletNexus  ");
		if (!this->m_hwnd)
			throw std::runtime_error("Failed to find the game's window.");

		g_pointers = this;
	}

	pointers::~pointers()
	{
		free(this->m_swapchain_methods);
		::DestroyWindow(this->m_window);
		g_pointers = nullptr;
	}

	bool pointers::get_swapchain()
	{
		WNDCLASSEX windowClass;
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = DefWindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandle(NULL);
		windowClass.hIcon = NULL;
		windowClass.hCursor = NULL;
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = L"Kiero";
		windowClass.hIconSm = NULL;

		::RegisterClassEx(&windowClass);

		this->m_window = ::CreateWindow(windowClass.lpszClassName, L"Kiero DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

		if (this->m_window == NULL)
		{
			return false;
		}

		HMODULE libD3D11 = ::GetModuleHandle(L"d3d11.dll");
		if (libD3D11 == NULL)
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return false;
		}

		void* D3D11CreateDeviceAndSwapChain = ::GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain");
		if (D3D11CreateDeviceAndSwapChain == NULL)
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return false;
		}

		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1, };

		DXGI_RATIONAL refreshRate;
		refreshRate.Numerator = 60;
		refreshRate.Denominator = 1;

		DXGI_MODE_DESC bufferDesc;
		bufferDesc.Width = 100;
		bufferDesc.Height = 100;
		bufferDesc.RefreshRate = refreshRate;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc = sampleDesc;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = this->m_window;
		swapChainDesc.Windowed = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = ((long(__stdcall*)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**))(D3D11CreateDeviceAndSwapChain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &this->m_swapchain, &this->m_d3d_device, &featureLevel, &this->m_d3d_context);
		if (FAILED(hr))
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return false;
		}

		::memcpy(this->m_swapchain_methods, *(void***)this->m_swapchain, sizeof(m_swapchain_methods));

		this->m_swapchain->Release();
		this->m_swapchain = NULL;

		this->m_d3d_device->Release();
		this->m_d3d_device = NULL;

		this->m_d3d_context->Release();
		this->m_d3d_context = NULL;

		::DestroyWindow(this->m_window);
		::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

		return true;
	}
}