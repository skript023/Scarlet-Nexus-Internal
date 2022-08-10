#pragma once
#include "unreal_engine_utility.hpp"

namespace big::player
{
	inline void set_velocity_jump(float number)
	{
		if (auto character_base = unreal_engine::get_character_base())
		{
			character_base->m_ability->m_jump_velocity = number;
		}
	}

	inline void set_player_health(int health)
	{
		if (auto player_params = unreal_engine::get_character_base())
		{
			player_params->m_character_params->m_health = health;
		}
	}

	inline int get_player_max_health()
	{
		if (auto player_params = unreal_engine::get_character_base())
		{
			return player_params->m_character_params->m_max_health;
		}

		return 0;
	}

	inline void player_health_regeneration(bool activate)
	{
		if (activate)
		{
			if (auto player_params = unreal_engine::get_character_base())
			{
				if (player_params->m_character_params->m_health <= get_player_max_health())
				{
					player_params->m_character_params->m_health += 100;
				}
			}
		}
	}

	inline void infinite_health(bool activate)
	{
		if (activate)
		{
			if (auto player_params = unreal_engine::get_character_base())
			{
				player_params->m_character_params->m_health = get_player_max_health();
			}
		}
	}

	inline void set_ignore_damage(bool activate)
	{
		uintptr_t base = 0x0;
		if (auto player_params = unreal_engine::get_character_base())
		{
			base = player_params->m_player_damage;

			if (activate)
			{
				player_params->m_player_damage = 0;
			}
			else
			{
				if (player_params->m_player_damage != 0)
					return;

				if (base != 0)
					player_params->m_player_damage = base;
			}
		}
	}

	inline void set_player_gauge_regeneration(float pychic)
	{
		if (auto pychic_addr = unreal_engine::get_character_base())
			pychic_addr->m_gauge_base->m_telekinetic_regen = pychic;
	}

	inline void set_player_gauge(float pychic)
	{
		if (auto pychic_addr = unreal_engine::get_character_base())
			pychic_addr->m_gauge_base->m_telekinetic_gauge = pychic;
	}

	inline float get_player_max_gauge()
	{
		if (auto pychic_addr = unreal_engine::get_character_base())
			return pychic_addr->m_gauge_base->m_max_telekinetic_gauge;

		return 0.f;
	}

	inline float get_player_exp_multiplier()
	{
		if (auto pychic_addr = unreal_engine::get_character_base())
			return pychic_addr->m_gauge_base->m_battle_bonus_rate;

		return 0.f;
	}

	inline void remove_sas_cooldown(bool activate)
	{
		if (auto base = unreal_engine::get_character_base())
		{
			if (auto sas_base = base->m_sas_base)
			{
				auto num = sas_base->m_num_skill;
				auto recast = sas_base->m_sas_recast;
				if (activate)
				{
					for (int i = 0; i < num; i++)
						recast->m_skill_cooldown[i].m_selected_skill = false;
				}
				else
				{
					for (int i = 0; i < num; i++)
						recast->m_skill_cooldown[i].m_selected_skill = true;
				}
			}
		}
	}

	inline void set_infinite_sas_duration(bool activate)
	{
		if (auto base = unreal_engine::get_character_base())
		{
			if (auto sas_duration = base->m_sas_state)
				sas_duration->m_disable_sas_duration = activate;
		}
	}

	inline void set_infinite_brain_drive(bool activate)
	{
		if (activate)
		{
			if (auto base = unreal_engine::get_character_base())
			{
				base->m_brain_drive->m_infinite_brain_drive = 0.0f;
			}
		}
	}

	inline void instant_brain_field(bool activate)
	{
		if (activate)
		{
			if (auto base = unreal_engine::get_character_base())
			{
				base->m_brain_drive->m_instant_brain_field = 0.0f;
			}
		}
	}

	inline void enter_brain_drive()
	{
		if (auto base = unreal_engine::get_character_base())
		{
			base->m_brain_drive->m_enter_brain_drive = true;
		}
	}

	inline void item_usage_no_cooldown(bool activate)
	{
		if (activate)
		{
			if (auto base = unreal_engine::get_character_base())
			{
				auto num = base->m_item_use_recast->m_num_item;
				auto item_recast = base->m_item_use_recast->m_item_recast;
				for (int i = 0; i < num; i++)
				{
					item_recast->m_item_list[i].m_selected_items = '\x00';
				}
			}
		}
	}

	inline void infinite_battle_points_ex(bool activate)
	{
		//if (activate)
			//*(uint8_t*)g_pointers->m_battle_points_handle = 0xC3;
		//else
			//*(uint8_t*)g_pointers->m_battle_points_handle = 0x48;
	}

	inline void modify_player_skill(int slot, int8_t skillId)
	{
		if (auto base = unreal_engine::get_character_base())
		{
			switch (slot)
			{
			case 1:
				base->m_sas_base->m_player_skill->m_skill.m_slot_3 = skillId;
				break;
			case 2:
				base->m_sas_base->m_player_skill->m_skill.m_slot_1 = skillId;
				break;
			case 3:
				base->m_sas_base->m_player_skill->m_skill.m_slot_2 = skillId;
				break;
			case 4:
				base->m_sas_base->m_player_skill->m_skill.m_slot_4 = skillId;
				break;
			case 5:
				base->m_sas_base->m_player_skill->m_skill.m_slot_7 = skillId;
				break;
			case 6:
				base->m_sas_base->m_player_skill->m_skill.m_slot_5 = skillId;
				break;
			case 7:
				base->m_sas_base->m_player_skill->m_skill.m_slot_6 = skillId;
				break;
			case 8:
				base->m_sas_base->m_player_skill->m_skill.m_slot_8 = skillId;
				break;
			}
		}
	}

	inline int8_t get_player_skill(int slot)
	{
		if (auto base = unreal_engine::get_character_base())
		{
			switch (slot)
			{
			case 1:
				return base->m_sas_base->m_player_skill->m_skill.m_slot_3;
			case 2:
				return base->m_sas_base->m_player_skill->m_skill.m_slot_1;
			case 3:
				return base->m_sas_base->m_player_skill->m_skill.m_slot_2;
			case 4:
				return base->m_sas_base->m_player_skill->m_skill.m_slot_4;
			case 5:
				return base->m_sas_base->m_player_skill->m_skill.m_slot_7;
			case 6:
				return base->m_sas_base->m_player_skill->m_skill.m_slot_5;
			case 7:
				return base->m_sas_base->m_player_skill->m_skill.m_slot_6;
			case 8:
				return base->m_sas_base->m_player_skill->m_skill.m_slot_8;
			}
		}

		return 0;
	}

	inline int get_player_credits()
	{
		if (auto base = unreal_engine::get_user_params())
		{
			return base->m_credit;
		}

		return 0;
	}

	inline void set_player_credits(int credits)
	{
		if (auto base = unreal_engine::get_user_params())
		{
			base->m_credit = credits;
		}
	}

	inline void infinite_player_credits(bool activate)
	{
		static const auto credits = get_player_credits();
		if (activate)
		{
			set_player_credits(credits);
		}
	}

	inline int get_player_battle_point()
	{
		if (auto base = unreal_engine::get_user_params())
		{
			return base->m_skill_point;
		}

		return 0;
	}

	inline void set_player_battle_point(int battlePoints)
	{
		if (auto base = unreal_engine::get_user_params())
		{
			base->m_skill_point = battlePoints;
		}
	}

	inline void infinite_battle_points(bool activate)
	{
		static const auto battle_points = get_player_credits();
		if (activate)
		{
			set_player_battle_point(battle_points);
		}
	}
}