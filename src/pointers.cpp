#include "common.hpp"
#include "logger.hpp"
#include "pointers.hpp"

namespace big
{
	pointers::pointers() : main_batch("pointer_cache")
	{
		if (!this->get_swapchain())
			LOG(WARNING) << "Failed get swapchain";
		
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
			m_battle_points_handle = ptr.as<void*>();
		});

		main_batch.add("Credits Handler", "48 89 ? ? ? 48 89 ? ? ? 48 89 ? ? ? 57 48 83 EC ? 4C 8B ? ? ? ? ? 33 F6", [this](memory::handle ptr)
		{
			m_credits_handle = ptr.as<void*>();
		});

		main_batch.add("Items Handler", "45 8D ? ? 45 89 ? ? 45 3B", [this](memory::handle ptr)
		{
			m_items_handle = ptr.as<void*>();
		});
		
		main_batch.add("Screen Resolution", "0F 5B D0 0F 5B C9 0F 14 D1 F2 41 0F 11 10 C3", [this](memory::handle ptr)
		{
			m_resolution = ptr.sub(12).rip().as<decltype(m_resolution)>(); //Alternative add(14).rip() 0F 95 C1 48 03 C8 48 89 4A 20 66 0F 6E 05 ? ? ? ? 66 0F 6E 0D ? ? ? ? 0F 5B D0 0F 5B C9 0F 14 D1 F2 41 0F 11 10 C3
		});
		
		main_batch.add("Return Address", "FF 23", [this](memory::handle ptr)
		{
			m_return_address = ptr.as<void*>();
		});

		main_batch.run(memory::module(nullptr));

		this->m_hwnd = FindWindow("UnrealWindow", "ScarletNexus  ");
		if (!this->m_hwnd)
			throw std::runtime_error("Failed to find the game's window.");

		g_pointers = this;
	}

	pointers::~pointers()
	{
		g_pointers = nullptr;
	}

	void pointers::update()
	{
		main_batch.update();
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
		windowClass.lpszClassName = "Kiero";
		windowClass.hIconSm = NULL;

		::RegisterClassEx(&windowClass);

		this->m_window = ::CreateWindow(windowClass.lpszClassName, "Kiero DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

		if (this->m_window == NULL)
		{
			return false;
		}

		HMODULE d3d11 = ::GetModuleHandle("d3d11.dll");
		if (d3d11 == NULL)
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return false;
		}

		void* create_device_and_swapchain = ::GetProcAddress(d3d11, "D3D11CreateDeviceAndSwapChain");
		if (create_device_and_swapchain == NULL)
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

		HRESULT hr = ((functions::create_d3d11_device_and_swapchain_t)(create_device_and_swapchain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &this->m_swapchain, &this->m_d3d_device, &featureLevel, &this->m_d3d_context);
		
		if (FAILED(hr))
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return false;
		}

		::memcpy(this->m_swapchain_methods, *(void***)this->m_swapchain, sizeof(m_swapchain_methods));

		this->m_swapchain->Release();
		this->m_swapchain = nullptr;

		this->m_d3d_device->Release();
		this->m_d3d_device = nullptr;

		this->m_d3d_context->Release();
		this->m_d3d_context = nullptr;

		::DestroyWindow(this->m_window);
		::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

		return true;
	}
}