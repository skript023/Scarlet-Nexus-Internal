#pragma once
#include "logger.hpp"
#include "imgui.h"

#include "class/vector.hpp"
#include "utility/utility.hpp"

namespace big
{
	class settings {
		nlohmann::json default_options;
		nlohmann::json options;
	public:
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
			bool input = false;
			bool censor = true;
			bool overlay = false;

			uint32_t open_key{ VK_INSERT };
			uint32_t back_key{ VK_NUMPAD0 };
			uint32_t enter_key{ VK_NUMPAD5 };
			uint32_t up_key{ VK_NUMPAD8 };
			uint32_t down_key{ VK_NUMPAD2 };
			uint32_t left_key{ VK_NUMPAD4 };
			uint32_t right_key{ VK_NUMPAD6 };
			uint32_t left_tab_key{ VK_NUMPAD7 };
			uint32_t right_tab_key{ VK_NUMPAD9 };

			Vector2 m_pos = { 25.f, 25.f };
			float m_width = 450.f;
			std::size_t m_option_per_page = 11;
			bool m_sounds = true;
			// Input
			std::int32_t m_open_delay = 200;
			std::int32_t m_back_delay = 300;
			std::int32_t m_enter_delay = 300;
			std::int32_t m_vectical_delay = 120;
			std::int32_t m_horizontal_delay = 120;
			std::int32_t m_tabbar_switch = 200;

			// Submenu bar
			Color m_submenu_bar_background_color{ 0, 0, 0, 150 };
			Color m_submenu_bar_text_color{ 255, 255, 255, 255 };

			//Options
			Color m_toggle_on_color{ 255, 255, 255, 200 };
			Color m_toggle_off_color{ 0, 0, 0, 150 };
			Color m_submenu_rect_color{ 255, 255, 255, 180 };
			Color m_option_selected_text_color{ 0, 0, 0, 255 };
			Color m_option_unselected_text_color{ 255, 255, 255, 255 };
			Color m_option_selected_background_color{ 255, 255, 255, 200 };
			Color m_option_unselected_background_color{ 0, 0, 0, 150 };

			//Footer
			Color m_footer_background_color{ 0, 0, 0, 150 };

			//Tabbar
			Color m_tab_unselected_text_color{ 255, 255, 255, 255 };
			Color m_tab_selected_text_color{ 0, 0, 0, 255 };
			Color m_tab_unselected_color{ 0, 0, 0, 150 };
			Color m_tab_selected_color{ 255, 255, 255, 200 };

			//Description
			Color description_background_color{ 0, 0, 0, 150 };
			Color description_text_color{ 255, 255, 255, 255 };

			//Sliderbar
			Color m_slider_track_color = { 255, 255, 255, 200 };
			Color m_slider_knob_color = { 0, 0, 0, 150 };

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(window,
				color,
				gui_scale,
				mouse_active,
				censor,
				overlay,
				open_key,
				back_key,
				enter_key,
				up_key,
				down_key,
				left_key,
				right_key,
				left_tab_key,
				right_tab_key,
				m_pos,
				m_width,
				m_option_per_page,
				m_sounds,
				m_open_delay,
				m_enter_delay,
				m_back_delay,
				m_horizontal_delay,
				m_vectical_delay,
				m_tabbar_switch,
				m_submenu_bar_background_color,
				m_submenu_bar_text_color,
				m_toggle_on_color,
				m_toggle_off_color,
				m_submenu_rect_color,
				m_option_selected_background_color,
				m_option_unselected_background_color,
				m_option_selected_text_color,
				m_option_unselected_text_color,
				m_footer_background_color,
				m_tab_selected_color,
				m_tab_unselected_color,
				m_tab_selected_text_color,
				m_tab_unselected_text_color,
				description_background_color,
				description_text_color
			)
		} window;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(settings, window, self, skill)
	};

	inline settings g_settings{};
}