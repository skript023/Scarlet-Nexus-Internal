#include "hooking.hpp"
#include "script_mgr.hpp"
#include "SDK/BattlePrototype_parameters.hpp"

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

				auto param = static_cast<SDK::Params::UserParamManager_SetBondsLevel*>(parms);

				param->Level = 6;
				param->AdjustExp = true;

				return g_hooking->m_process_event_hook.get_original<decltype(&process_event)>()(_this, function, &param);
			}
			if (function->get_fullname() == "Function BattlePrototype.MakeEventActor.AddBondsLevel")
			{
				LOG(INFO) << "Executed AddBondsLevel";

				auto param = static_cast<SDK::Params::UserParamManager_AddBondsValue*>(parms);

				param->Value = 6;

				return g_hooking->m_process_event_hook.get_original<decltype(&process_event)>()(_this, function, &param);
			}
			if (function->get_fullname() == "Function BattlePrototype.CharactersParameterComponent.SetCharacterIgnoreDamageFlag")
			{
				LOG(INFO) << "Executed SetCharacterIgnoreDamageFlag";
			}
			if (function->get_fullname() == "Function BattlePrototype.UserParamManager.SetSkillPoint")
			{
				LOG(INFO) << "Executed SetSkillPoint";
			}
		}

		return g_hooking->m_process_event_hook.get_original<decltype(&process_event)>()(_this, function, parms);
	}
}