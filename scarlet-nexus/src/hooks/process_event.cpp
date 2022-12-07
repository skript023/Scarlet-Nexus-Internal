#include "hooking.hpp"
#include "script_mgr.hpp"

namespace big
{
	void APIENTRY hooks::process_event(UObject* _this, UFunction* function, void* parms)
	{
		if (g_running)
		{
			if (function->get_name() == "ReceiveTick" || function->get_name() == "Received_NotifyTick")
			{

			}
		}
		return g_hooking->m_process_event_hook.get_original<decltype(&process_event)>()(_this, function, parms);
	}
}