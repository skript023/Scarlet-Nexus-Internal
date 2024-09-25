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
			if (function->get_fullname() == "Function BattlePrototype.BattleEnemyInterface.SetHp_Native")
			{
				struct Parameter
				{ 
					int set_health;
					bool ret;
				};

				auto param = static_cast<Parameter*>(parms);
				param->set_health = 0;

				LOG(HACKER) << "Setting health";

				return g_hooking->m_process_event_hook.get_original<decltype(&process_event)>()(_this, function, param);
			}
			if (function->get_fullname() == "Function BattlePrototype.CharactersParameterComponent.SetCharacterIgnoreDamageFlag")
			{
				struct SetCharacterIgnoreDamageFlag
				{ 
					int flag;
				};

				auto param = static_cast<SetCharacterIgnoreDamageFlag*>(parms);
				param->flag = 0;

				LOG(HACKER) << "Setting SetCharacterIgnoreDamageFlag";
				return g_hooking->m_process_event_hook.get_original<decltype(&process_event)>()(_this, function, param);
			}
		}

		return g_hooking->m_process_event_hook.get_original<decltype(&process_event)>()(_this, function, parms);
	}
}