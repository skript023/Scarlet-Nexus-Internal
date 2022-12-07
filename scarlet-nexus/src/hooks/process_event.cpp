#include "hooking.hpp"
#include "script_mgr.hpp"
#include "memory/module.hpp"

namespace big
{
	DWORD64 base_address = memory::module("ScarletNexus-Win64-Shipping.exe").begin().as<DWORD64>();

	void APIENTRY hooks::process_event(UObject* _this, UFunction* function, void* parms)
	{
		if (g_running)
		{
			if (function->get_fullname() == "Function BP_SASCutIn.BP_SASCutIn_C.ReceiveTick")
			{
				g_script_mgr.tick();
			}
		}
		return g_hooking->m_process_event_hook.get_original<decltype(&process_event)>()(_this, function, parms);
	}
}