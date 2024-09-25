#include "imgui.h"
#include "script.hpp"
#include "submenu.hpp"
#include "settings.hpp"
#include "player_stats.h"
#include "utility/player.hpp"

namespace big
{
	void player_stat::render_menu()
	{
        if (auto character = unreal_engine::get_character_base())
        {
            if (auto stats = character->m_character_stats)
            {
                ImGui::SliderFloat("DAMAGE FACTOR", &stats->m_dmg_factor, 0.0f, 9999.f);

                ImGui::SliderFloat("OBJECT DAMAGE FACTOR", &stats->m_obj_dmg_factor, 0.0f, 9999.f);

                ImGui::SliderFloat("FIRE DAMAGE FACTOR", &stats->m_fire_dmg_factor, 0.0f, 9999.f);

                ImGui::SliderFloat("ELECTRIC DAMAGE FACTOR", &stats->m_electric_dmg_factor, 0.0f, 9999.f);

                ImGui::SliderFloat("RANDOM FACTOR_MIN", &stats->m_random_factor_min, 0.0f, 9999.f);

                ImGui::SliderFloat("RANDOM FACTOR MAX", &stats->m_random_factor_max, 0.0f, 9999.f);

                ImGui::SliderFloat("CRITICAL FACTOR", &stats->m_critical_factor, 0.0f, 9999.f);

                ImGui::SliderFloat("CRITICAL CRASH FACTOR", &stats->m_critical_crash_factor, 0.0f, 9999.f);

                ImGui::SliderFloat("BAD STATE UP FACTOR", &stats->m_bad_up_factor, 0.0f, 9999.f);

                ImGui::SliderFloat("BAD STATE DOWN FACTOR", &stats->m_bad_down_factor, 0.0f, 9999.f);

                ImGui::SliderFloat("CRASH FACTOR", &stats->m_crash_factor, 0.0f, 9999.f);

                ImGui::SliderFloat("ARMOR DAMAGE UP FACTOR", &stats->m_armor_dmg_factor, 0.0f, 9999.f);

                ImGui::SliderFloat("COPY PSYCHIC ARMOR FACTOR", &stats->m_copy_psy_armor_factor, 0.0f, 9999.f);

                ImGui::SliderFloat("PSYCHICFIELD DAMAGE FACTOR BOSS", &stats->m_psy_dmg_factor_boss, 0.0f, 9999.f);
            }

        }
	}
    void player_stat::render_submenu()
    {
        g_ui_manager->add_submenu<regular_submenu>("Player", SubmenuStats, [](regular_submenu* sub)
        {
            if (auto character = unreal_engine::get_character_base())
            {
                if (auto stats = character->m_character_stats)
                {
                    sub->add_option<number_option<float>>("DAMAGE FACTOR", nullptr, &stats->m_dmg_factor, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("OBJECT DAMAGE FACTOR", nullptr, &stats->m_obj_dmg_factor, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("FIRE DAMAGE FACTOR", nullptr, &stats->m_fire_dmg_factor, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("ELECTRIC DAMAGE FACTOR", nullptr, &stats->m_electric_dmg_factor, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("RANDOM FACTOR MIN", nullptr, &stats->m_random_factor_min, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("RANDOM FACTOR MAX", nullptr, &stats->m_random_factor_max, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("CRITICAL FACTOR", nullptr, &stats->m_critical_factor, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("CRITICAL CRASH FACTOR", nullptr, &stats->m_critical_crash_factor, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("BAD STATE UP FACTOR", nullptr, &stats->m_bad_up_factor, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("BAD STATE DOWN FACTOR", nullptr, &stats->m_bad_down_factor, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("CRASH FACTOR", nullptr, &stats->m_crash_factor, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("ARMOR DAMAGE UP FACTOR", nullptr, &stats->m_armor_dmg_factor, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("COPY PSYCHIC ARMOR FACTOR", nullptr, &stats->m_copy_psy_armor_factor, 0.f, 9999.f, 0.1f, 1);

                    sub->add_option<number_option<float>>("PSYCHICFIELD DAMAGE FACTOR BOSS", nullptr, &stats->m_psy_dmg_factor_boss, 0.f, 9999.f, 0.1f, 1);
                }

            }
        });
    }
}