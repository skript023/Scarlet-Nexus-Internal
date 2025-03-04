#pragma once
#include "common.hpp"
#include "memory/all.hpp"
#include "function_types.hpp"
#include "class/engine_sdk.hpp"
#include <class/vector.hpp>
#include <unreal_engine/SDK/Engine_classes.hpp>

namespace big
{
	class pointers
	{
		memory::pattern_batch main_batch;
	public:
		explicit pointers();
		~pointers();

		void update();
	public:
		HWND m_hwnd{};

		void* m_swapchain_methods[19];

		EngineBase** m_engine{};
		FUObjectArray* m_object_array{};
		FNamePool* m_name{};
		iVector2* m_resolution{};
		functions::process_event_t m_process_event{};
		void* m_battle_points_handle;
		void* m_credits_handle;
		void* m_items_handle;
		void* m_return_address;
	private:
		functions::create_d3d11_device_and_swapchain_t create_device_and_swapchain{};
		bool get_swapchain();
		bool swapchain_found = false;
		HWND m_window;
		IDXGISwapChain* m_swapchain = nullptr;
		ID3D11Device* m_d3d_device = nullptr;
		ID3D11DeviceContext* m_d3d_context = nullptr;
	public:
		inline DWORD get_process_id(LPCSTR ProcessName)
		{
			PROCESSENTRY32 pt;
			HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			pt.dwSize = sizeof(PROCESSENTRY32);
			if (Process32First(hsnap, &pt))
			{
				do
				{
					if (!lstrcmpiA(pt.szExeFile, ProcessName))
					{
						CloseHandle(hsnap);
						return pt.th32ProcessID;
					}
				} while (Process32Next(hsnap, &pt));
			}
			CloseHandle(hsnap);
			return 0;
		}
	};

	inline pointers *g_pointers{};
}
