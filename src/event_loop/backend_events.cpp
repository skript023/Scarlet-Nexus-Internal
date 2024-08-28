#include "backend_events.hpp"
#include "script.hpp"

#include "utility/player.hpp"
#include "server/server_module.hpp"


namespace big
{
	void backend_events::features()
	{
		if (unreal_engine::get_character_base())
		{
			if (g_settings->self.infinite_psychic)
			{
				player::set_player_gauge_regeneration(1000.f);
			}

			player::remove_sas_cooldown(g_settings->self.no_sas_cooldown);
			player::set_infinite_sas_duration(g_settings->self.infinite_sas_duration);

			player::set_infinite_brain_drive(g_settings->self.infinite_brain_dive);
			player::instant_brain_field(g_settings->self.instant_brain_field);
			player::infinite_battle_points(g_settings->self.infinite_battle_point);
			player::item_usage_no_cooldown(g_settings->self.no_items_cooldown);
			player::infinite_player_credits(g_settings->self.infinite_credits);
			player::infinite_health(g_settings->self.infinite_health);
		}
	}

	void backend_events::player_skill_event()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				g_settings->skill.slot_1 = player::get_player_skill(1);
				g_settings->skill.slot_2 = player::get_player_skill(2);
				g_settings->skill.slot_3 = player::get_player_skill(3);
				g_settings->skill.slot_4 = player::get_player_skill(4);
				g_settings->skill.slot_5 = player::get_player_skill(5);
				g_settings->skill.slot_6 = player::get_player_skill(6);
				g_settings->skill.slot_7 = player::get_player_skill(7);
				g_settings->skill.slot_8 = player::get_player_skill(8);
			}
			EXCEPT_CLAUSE
			script::get_current()->yield();
		}
	}

	void backend_events::script_func()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				features();
				g_settings->attempt_save();
				g_server_module->get_alpha()->poll();
			} 
			EXCEPT_CLAUSE
			script::get_current()->yield();
		}
	}
}