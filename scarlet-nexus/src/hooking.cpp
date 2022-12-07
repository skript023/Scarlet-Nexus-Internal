#include "common.hpp"
#include "function_types.hpp"
#include "logger.hpp"
#include "gui.hpp"
#include "hooking.hpp"
#include "memory/module.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include <MinHook.h>


namespace big
{
	hooking::hooking()
	{
		detour_hook_helper::add<hooks::swapchain_present>("Swap Chain Present", g_pointers->m_swapchain_methods[hooks::swapchain_present_index]);
		detour_hook_helper::add<hooks::swapchain_resizebuffers>("Swap Chain Resize Buffers", g_pointers->m_swapchain_methods[hooks::swapchain_resizebuffers_index]);
		detour_hook_helper::add<hooks::set_cursor_pos>("Set Cursor Pos", memory::module("user32.dll").get_export("SetCursorPos").as<void*>());
		detour_hook_helper::add<hooks::convert_thread_to_fiber>("Convert Thread To Fiber", memory::module("kernel32.dll").get_export("ConvertThreadToFiber").as<void*>());
		detour_hook_helper::add<hooks::process_event>("Process Event", g_pointers->m_process_event);
		
		g_hooking = this;
	}

	hooking::~hooking()
	{
		if (m_enabled)
			disable();

		g_hooking = nullptr;
	}

	void hooking::enable()
	{
		m_og_wndproc = WNDPROC(SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, LONG_PTR(&hooks::wndproc)));

		for (const auto& detour_hook_helper : m_detour_hook_helpers)
		{
			detour_hook_helper->m_detour_hook->enable();
		}

		MH_ApplyQueued();

		m_enabled = true;
	}

	void hooking::disable()
	{
		m_enabled = false;

		for (const auto& detour_hook_helper : m_detour_hook_helpers)
		{
			detour_hook_helper->m_detour_hook->disable();
		}

		SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_og_wndproc));

		MH_ApplyQueued();

		for (const auto& detour_hook_helper : m_detour_hook_helpers)
		{
			delete detour_hook_helper;
		}

		m_detour_hook_helpers.clear();
	}

	hooking::detour_hook_helper::~detour_hook_helper()
	{
		delete m_detour_hook;
	}

	void hooking::detour_hook_helper::enable_hook_if_hooking_is_already_running()
	{
		if (g_hooking && g_hooking->m_enabled)
		{
			if (m_on_hooking_available)
			{
				m_detour_hook->set_target_and_create_hook(m_on_hooking_available());
			}

			m_detour_hook->enable();
			MH_ApplyQueued();
		}
	}

	minhook_keepalive::minhook_keepalive()
	{

	}

	minhook_keepalive::~minhook_keepalive()
	{

	}

	void *hooks::convert_thread_to_fiber(void *param)
	{
		if (IsThreadAFiber())
		{
			return GetCurrentFiber();
		}

		return g_hooking->get_original<&convert_thread_to_fiber>()(param);
	}

	LRESULT hooks::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (g_running)
		{
			g_renderer->wndproc(hwnd, msg, wparam, lparam);
		}

		return CallWindowProcW(g_hooking->m_og_wndproc, hwnd, msg, wparam, lparam);
	}

	BOOL hooks::set_cursor_pos(int x, int y)
	{
		if (g_gui.m_opened)
			return true;

		return g_hooking->get_original<&set_cursor_pos>()(x, y);
	}
}
