#pragma once
#include "common.hpp"
#include "function_types.hpp"
#include "class/engine_sdk.hpp"

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
		functions::process_event_t m_process_event{};
		PVOID m_battle_points_handle;
		PVOID m_credits_handle;
		PVOID m_items_handle;
	private:
		bool get_swapchain();
		bool swapchain_found = false;
		HWND m_window;
		IDXGISwapChain* m_swapchain;
		ID3D11Device* m_d3d_device;
		ID3D11DeviceContext* m_d3d_context;
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
