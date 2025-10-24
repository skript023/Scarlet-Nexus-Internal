#include "../view.hpp"
#include "script.hpp"
#include "ufunction.hpp"
#include "utility/player.hpp"
#include "SDK/BattlePrototype_classes.hpp"

namespace big
{
    void view::player_submenu()
    {
        canvas::add_submenu<regular_submenu>("Player", SubmenuPlayer, [](regular_submenu* sub)
        {
            sub->add_option<reguler_option>("Enter Brain Dive", nullptr, []
            {
                player::enter_brain_drive();
            });

            sub->add_option<reguler_option>("Set Max Bond Level", nullptr, [] {
                if (auto userPtr = reinterpret_cast<SDK::UUserParamManager*>(unreal_engine::get_user_params())) 
                {
                    userPtr->AddBondsValue(SDK::EPlayerID::Max, SDK::EPlayerID::Max, 1000000);
                    userPtr->SetTeamBondsLevel(SDK::EPlayerID::Ch0100, 6);
                }
            });

            sub->add_option<bool_option<bool>>("Infinite Psychic Gauge", nullptr, &g_settings.self.infinite_psychic);
            sub->add_option<bool_option<bool>>("SAS No Cooldown", nullptr, &g_settings.self.no_sas_cooldown);
            sub->add_option<bool_option<bool>>("Infinite SAS Duration", nullptr, &g_settings.self.infinite_sas_duration);

            sub->add_option<bool_option<bool>>("No Item Use Cooldown", nullptr, &g_settings.self.no_items_cooldown);
            sub->add_option<bool_option<bool>>("Infinite Battle Point", nullptr, &g_settings.self.infinite_battle_point);
            sub->add_option<bool_option<bool>>("Infinite Credits", nullptr, &g_settings.self.infinite_credits);

            sub->add_option<bool_option<bool>>("Infinite Brain Drive", nullptr, &g_settings.self.infinite_brain_dive);
            sub->add_option<bool_option<bool>>("Instant Brain Field", nullptr, &g_settings.self.instant_brain_field);
            sub->add_option<bool_option<bool>>("Infinite Health", nullptr, &g_settings.self.infinite_health);
            
            sub->add_option<bool_option<bool>>("Ignore Damage", nullptr, &g_settings.self.ignore_damage);

            static UC::int32 hp = player::get_player_health();
            static UC::int32 max_hp = player::get_player_max_health();

            sub->add_option<number_option<UC::int32>>("Set Player HP", nullptr, &hp, 0, max_hp, 1, 3, true, "", "", [] {
                player::set_player_health(hp);
            });

            if (auto pg = player::get_player_gauge())
                sub->add_option<number_option<float>>("Set Psychic Gauge", nullptr, pg, 0.f, 1000.f, 0.1f, 1);

            if (auto bp = player::get_player_battle_point())
                sub->add_option<number_option<int>>("Set Battle Point", nullptr, bp, 0, INT32_MAX);

            if (auto cr = player::get_player_credits())
                sub->add_option<number_option<int>>("Set Credits", nullptr, cr, 0, INT32_MAX);

            if (auto sp = player::get_skill_point())
                sub->add_option<number_option<int>>("Set Skill Point", nullptr, sp, 0, INT32_MAX);
            

            SDK::UWorld::GetWorld();
            // static std::int64_t int64Test{ 420 };
            // sub->add_option<number_option<std::int64_t>>("Int64", nullptr, &int64Test, 0, 1000, 10);

            if (auto exp = player::player_exp_multiplier())
                sub->add_option<number_option<float>>("EXP Multiplier", nullptr, exp, 0.f, 1000.f, 0.1f, 1);

            static std::vector<std::uint64_t> vector{ 1, 2, 3 };
            static std::size_t vectorPos{};

            static std::vector<std::string> skill_list_v = { "Pyrokinesis","Sclerokinesis","Clairvoyance","Teleportation","Invisibility","Electrokinesis","Duplication","Hypervelocity","Psychokinesis","Psychokinesis","No Skill" };
            static std::size_t skill_pos{};
            
            sub->add_option<choose_option<std::string, int>>("Skill Slot 1", nullptr, &skill_list_v, &g_settings.skill.slot_1, true, [] {
                player::modify_player_skill(1, g_settings.skill.slot_1);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 2", nullptr, &skill_list_v, &g_settings.skill.slot_2, true, [] {
                player::modify_player_skill(2, g_settings.skill.slot_2);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 3", nullptr, &skill_list_v, &g_settings.skill.slot_3, true, [] {
                player::modify_player_skill(3, g_settings.skill.slot_3);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 4", nullptr, &skill_list_v, &g_settings.skill.slot_4, true, [] {
                player::modify_player_skill(4, g_settings.skill.slot_4);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 5", nullptr, &skill_list_v, &g_settings.skill.slot_5, true, [] {
                player::modify_player_skill(5, g_settings.skill.slot_5);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 6", nullptr, &skill_list_v, &g_settings.skill.slot_6, true, [] {
                player::modify_player_skill(6, g_settings.skill.slot_6);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 7", nullptr, &skill_list_v, &g_settings.skill.slot_7, true, [] {
                player::modify_player_skill(7, g_settings.skill.slot_7);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 8", nullptr, &skill_list_v, &g_settings.skill.slot_8, true, [] {
                player::modify_player_skill(8, g_settings.skill.slot_8);
            });
            // sub->add_option<choose_option<std::uint64_t, std::size_t>>("Vector", nullptr, &vector, &vectorPos);
        });
    }
}