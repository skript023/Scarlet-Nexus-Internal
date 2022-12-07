#pragma once
#include "common.hpp"
#include "function_types.hpp"
#include "class/engine_sdk.hpp"
#include <class/vector.hpp>

namespace big
{
	class pointers
	{
	public:
		explicit pointers();
		~pointers();
	public:
		HWND m_hwnd{};

		void* m_swapchain_methods[19];

		EngineBase** m_engine{};
		FUObjectArray* m_object_array{};
		FNamePool* m_name{};
		iVector2* m_resolution{};
		functions::process_event_t m_process_event{};
		PVOID m_battle_points_handle;
		PVOID m_credits_handle;
		PVOID m_items_handle;
		PVOID m_return_address;
		PVOID m_main_hook;
	private:
		functions::create_d3d11_device_and_swapchain_t create_device_and_swapchain{};
		bool get_swapchain();
		bool swapchain_found = false;
		HWND m_window;
		IDXGISwapChain* m_swapchain = nullptr;
		ID3D11Device* m_d3d_device = nullptr;
		ID3D11DeviceContext* m_d3d_context = nullptr;
	public:
		inline DWORD get_process_id(LPCWSTR ProcessName)
		{
			PROCESSENTRY32 pt;
			HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			pt.dwSize = sizeof(PROCESSENTRY32);
			if (Process32First(hsnap, &pt))
			{
				do
				{
					if (!lstrcmpi(pt.szExeFile, ProcessName))
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
