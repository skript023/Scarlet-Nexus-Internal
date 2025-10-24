#include "imgui.h"
#include "script.hpp"
#include "../view.hpp"
#include "utility/player.hpp"

namespace big
{
    void view::stats_submenu()
    {
        canvas::add_submenu<regular_submenu>("Stats", SubmenuStats, [](regular_submenu* sub)
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