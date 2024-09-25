#include "hooking.hpp"
#include "script_mgr.hpp"

namespace big
{
	void APIENTRY hooks::process_event(UObject* _this, UFunction* function, void* parms)
	{
		if (g_running)
		{
			if (function->get_fullname() == "Function BP_SASCutIn.BP_SASCutIn_C.ReceiveTick")
			{
				g_script_mgr.tick();
			}
			if (function->get_fullname() == "Function BattlePrototype.UserParamManager.SetBondsLevel")
			{
				LOG(INFO) << "Executed SetBondsLevel";
			}
			if (function->get_fullname() == "Function BattlePrototype.CharactersParameterComponent.SetCharacterIgnoreDamageFlag")
			{
				LOG(INFO) << "Executed SetCharacterIgnoreDamageFlag";
			}
		}

		return g_hooking->m_process_event_hook.get_original<decltype(&process_event)>()(_this, function, parms);
	}
}