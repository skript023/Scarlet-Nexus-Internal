#include "imgui.h"
#include <script.hpp>
#include "player_menu.h"
#include "utility/player.hpp"
#include "settings.hpp"

namespace big
{
    void player_menu::add_value()
    {
        slot_1 = player::get_player_skill(1);
        slot_2 = player::get_player_skill(2);
        slot_3 = player::get_player_skill(3);
        slot_4 = player::get_player_skill(4);
        slot_5 = player::get_player_skill(5);
        slot_6 = player::get_player_skill(6);
        slot_7 = player::get_player_skill(7);
        slot_8 = player::get_player_skill(8);
        m_exp_multiplier = player::get_player_exp_multiplier();
    }

	void player_menu::render_menu()
	{
        if (ImGui::BeginTabItem("Player Option"))
        {
            add_value();
            auto end = std::chrono::system_clock::now();
            std::time_t date = std::chrono::system_clock::to_time_t(end);

            ImGui::Text("%s", std::ctime(&date));

            ImGui::BeginGroup();

            ImGui::Checkbox("Infinite Psychic Gauge", &g_settings->self.infinite_psychic);
            ImGui::Checkbox("SAS No Cooldown", &g_settings->self.no_sas_cooldown);
            ImGui::Checkbox("Infinite SAS Duration", &g_settings->self.infinite_sas_duration);

            ImGui::EndGroup();

            ImGui::SameLine(230.f);

            ImGui::BeginGroup();

            ImGui::Checkbox("No Item Use Cooldown", &g_settings->self.no_items_cooldown);
            ImGui::Checkbox("Infinite Battle Point", &g_settings->self.infinite_battle_point);
            ImGui::Checkbox("Infinite Credits", &g_settings->self.infinite_credits);

            ImGui::EndGroup();

            ImGui::SameLine(480.f);

            ImGui::BeginGroup();

            ImGui::Checkbox("Infinite Brain Drive", &g_settings->self.infinite_brain_dive);
            ImGui::Checkbox("Instant Brain Field", &g_settings->self.instant_brain_field);
            ImGui::Checkbox("Infinite Health", &g_settings->self.infinite_health);

            ImGui::EndGroup();

            if (ImGui::Button("Enter Brain Drive"))
            {
                player::enter_brain_drive();
            }

            ImGui::PushItemWidth(200.f);

            if (ImGui::SliderFloat("EXP Multiplier", &m_exp_multiplier, 1.0f, 1000.f))
                unreal_engine::get_character_base()->m_gauge_base->m_battle_bonus_rate = m_exp_multiplier;

            if (ImGui::Combo("Skill Slot 1", &slot_1, skill_list, IM_ARRAYSIZE(skill_list)))
                player::modify_player_skill(1, slot_1);

            if (ImGui::Combo("Skill Slot 2", &slot_2, skill_list, IM_ARRAYSIZE(skill_list)))
                player::modify_player_skill(2, slot_2);

            if (ImGui::Combo("Skill Slot 3", &slot_3, skill_list, IM_ARRAYSIZE(skill_list)))
                player::modify_player_skill(3, slot_3);

            if (ImGui::Combo("Skill Slot 4", &slot_4, skill_list, IM_ARRAYSIZE(skill_list)))
                player::modify_player_skill(4, slot_4);

            if (ImGui::Combo("Skill Slot 5", &slot_5, skill_list, IM_ARRAYSIZE(skill_list)))
                player::modify_player_skill(5, slot_5);

            if (ImGui::Combo("Skill Slot 6", &slot_6, skill_list, IM_ARRAYSIZE(skill_list)))
                player::modify_player_skill(6, slot_6);

            if (ImGui::Combo("Skill Slot 7", &slot_7, skill_list, IM_ARRAYSIZE(skill_list)))
                player::modify_player_skill(7, slot_7);

            if (ImGui::Combo("Skill Slot 8", &slot_8, skill_list, IM_ARRAYSIZE(skill_list)))
                player::modify_player_skill(8, slot_8);

            ImGui::PopItemWidth();

            ImGui::EndTabItem();
        }
	}
}