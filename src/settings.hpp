#pragma once
#include "logger.hpp"
#include "imgui.h"

namespace big
{
	class settings;
	inline settings* g_settings{};

	class settings {
		nlohmann::json default_options;
		nlohmann::json options;
	public:
		settings()
		{
			g_settings = this;
		}

		~settings()
		{
			g_settings = nullptr;
		}

		void attempt_save();

		bool load();
	private:
		const char* settings_location = "\\Scarlet Nexus Trainer\\settings.json";

		bool deep_compare(nlohmann::json& current_settings, const nlohmann::json& default_settings, bool compare_value = false);
		bool save();

		bool write_default_config();
	public:
		struct self
		{
			bool infinite_psychic{ false };
			bool no_sas_cooldown{ false };
			bool infinite_sas_duration{ false };
			bool infinite_brain_dive{ false };
			bool instant_brain_field{ false };
			bool infinite_battle_point{ false };
			bool no_items_cooldown{ false };
			bool enter_brain_dive{ false };
			bool infinite_credits{ false };
			bool infinite_health{ false };
			bool ignore_damage{ false };

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(self, infinite_psychic, no_sas_cooldown, infinite_sas_duration, infinite_brain_dive, instant_brain_field, infinite_battle_point, no_items_cooldown, enter_brain_dive, infinite_credits, infinite_health, ignore_damage)
		} self;

		struct skill_set
		{
			int slot_1;
			int slot_2;
			int slot_3;
			int slot_4;
			int slot_5;
			int slot_6;
			int slot_7;
			int slot_8;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(skill_set, slot_1, slot_2, slot_3, slot_4, slot_5, slot_6, slot_7, slot_8)
		} skill;

		struct window
		{
			ImU32 color = 3357612055;
			float gui_scale = 1.f;

			ImFont* font_title = nullptr;
			ImFont* font_sub_title = nullptr;
			ImFont* font_small = nullptr;
			ImFont* font_icon = nullptr;

			bool switched_view = true;
			bool mouse_active = false;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(window, color, gui_scale, mouse_active)
		} window;
	

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(settings, window, self, skill)
	};
}