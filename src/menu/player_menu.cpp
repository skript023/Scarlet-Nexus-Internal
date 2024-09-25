#include "imgui.h"
#include "script.hpp"
#include "submenu.hpp"
#include "settings.hpp"
#include "player_menu.h"
#include "ufunction.hpp"
#include "utility/player.hpp"
#include "SDK/BattlePrototype_classes.hpp"

namespace big
{
    void player_menu::add_value()
    {
        
    }

	void player_menu::render_menu()
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

        ImGui::SameLine(240.f);

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

        ImGui::SliderFloat("EXP Multiplier", player::player_exp_multiplier(), 1.0f, 1000.f);

        if (ImGui::Combo("Skill Slot 1", &g_settings->skill.slot_1, skill_list, IM_ARRAYSIZE(skill_list)))
            player::modify_player_skill(1, g_settings->skill.slot_1);

        if (ImGui::Combo("Skill Slot 2", &g_settings->skill.slot_2, skill_list, IM_ARRAYSIZE(skill_list)))
            player::modify_player_skill(2, g_settings->skill.slot_2);

        if (ImGui::Combo("Skill Slot 3", &g_settings->skill.slot_3, skill_list, IM_ARRAYSIZE(skill_list)))
            player::modify_player_skill(3, g_settings->skill.slot_3);

        if (ImGui::Combo("Skill Slot 4", &g_settings->skill.slot_4, skill_list, IM_ARRAYSIZE(skill_list)))
            player::modify_player_skill(4, g_settings->skill.slot_4);

        if (ImGui::Combo("Skill Slot 5", &g_settings->skill.slot_5, skill_list, IM_ARRAYSIZE(skill_list)))
            player::modify_player_skill(5, g_settings->skill.slot_5);

        if (ImGui::Combo("Skill Slot 6", &g_settings->skill.slot_6, skill_list, IM_ARRAYSIZE(skill_list)))
            player::modify_player_skill(6, g_settings->skill.slot_6);

        if (ImGui::Combo("Skill Slot 7", &g_settings->skill.slot_7, skill_list, IM_ARRAYSIZE(skill_list)))
            player::modify_player_skill(7, g_settings->skill.slot_7);

        if (ImGui::Combo("Skill Slot 8", &g_settings->skill.slot_8, skill_list, IM_ARRAYSIZE(skill_list)))
            player::modify_player_skill(8, g_settings->skill.slot_8);

        ImGui::PopItemWidth();
	}
    void player_menu::render_submenu()
    {
        g_ui_manager->add_submenu<regular_submenu>("Player", SubmenuPlayer, [](regular_submenu* sub)
        {
            sub->add_option<reguler_option>("Enter Brain Dive", nullptr, []
            {
                // auto setHP = g_ufunction->get_native("Function BattlePrototype.BattleEnemyInterface.SetHp_Native");
                // auto UIHealthControl = g_ufunction->get_class("Class BattlePrototype.UIHealthControl");

                // if (setHP && UIHealthControl)
                // {
                //     struct Parameter 
                //     { 
                //         int set_health;
                //         bool ret;
                //     } parameter;

                //     parameter.set_health = 0;

                //     ufunction::execute_native_function(UIHealthControl, "Function BattlePrototype.BattleEnemyInterface.SetHp_Native", &parameter);

                //     LOG(INFO) << "Object: " << setHP;
                // }
                player::enter_brain_drive();
            });

            sub->add_option<reguler_option>("Set Max Bond Level", nullptr, [] {
                SDK::UUserParamManager::GetDefaultObj()->SetBondsLevel(SDK::EPlayerID::Max, SDK::EPlayerID::FriendMax, 10, false);
                SDK::UUserParamManager::GetDefaultObj()->SetTeamBondsLevel(SDK::EPlayerID::Max, 8);

                LOG(INFO) << "Bond Level is " << SDK::UUserParamManager::GetDefaultObj()->GetBondsLevel(SDK::EPlayerID::FriendMax, SDK::EPlayerID::FriendMax);
                LOG(INFO) << "Team Bond Level is " << SDK::UUserParamManager::GetDefaultObj()->GetTeamBondsLevel(SDK::EPlayerID::Ch0100);
            });

            sub->add_option<bool_option<bool>>("Infinite Psychic Gauge", nullptr, &g_settings->self.infinite_psychic);
            sub->add_option<bool_option<bool>>("SAS No Cooldown", nullptr, &g_settings->self.no_sas_cooldown);
            sub->add_option<bool_option<bool>>("Infinite SAS Duration", nullptr, &g_settings->self.infinite_sas_duration);

            sub->add_option<bool_option<bool>>("No Item Use Cooldown", nullptr, &g_settings->self.no_items_cooldown);
            sub->add_option<bool_option<bool>>("Infinite Battle Point", nullptr, &g_settings->self.infinite_battle_point);
            sub->add_option<bool_option<bool>>("Infinite Credits", nullptr, &g_settings->self.infinite_credits);

            sub->add_option<bool_option<bool>>("Infinite Brain Drive", nullptr, &g_settings->self.infinite_brain_dive);
            sub->add_option<bool_option<bool>>("Instant Brain Field", nullptr, &g_settings->self.instant_brain_field);
            sub->add_option<bool_option<bool>>("Infinite Health", nullptr, &g_settings->self.infinite_health);
            
            sub->add_option<bool_option<bool>>("Ignore Damage", nullptr, &g_settings->self.ignore_damage);

            if (auto pg = player::get_player_gauge())
                sub->add_option<number_option<float>>("Set Psychic Gauge", nullptr, pg, 0.f, 1000.f, 0.1f, 1);

            if (auto bp = player::get_player_battle_point())
                sub->add_option<number_option<int>>("Set Battle Point", nullptr, bp, 0, 5000000);

            if (auto cr = player::get_player_credits())
                sub->add_option<number_option<int>>("Set Credits", nullptr, cr, 0, 5000000);

            // static std::int64_t int64Test{ 420 };
            // sub->add_option<number_option<std::int64_t>>("Int64", nullptr, &int64Test, 0, 1000, 10);

            if (auto exp = player::player_exp_multiplier())
                sub->add_option<number_option<float>>("EXP Multiplier", nullptr, exp, 0.f, 1000.f, 0.1f, 1);

            static std::vector<std::uint64_t> vector{ 1, 2, 3 };
            static std::size_t vectorPos{};

            static std::vector<std::string> skill_list_v = { "Pyrokinesis","Sclerokinesis","Clairvoyance","Teleportation","Invisibility","Electrokinesis","Duplication","Hypervelocity","Psychokinesis","Psychokinesis","No Skill" };
            static std::size_t skill_pos{};
            
            sub->add_option<choose_option<std::string, int>>("Skill Slot 1", nullptr, &skill_list_v, &g_settings->skill.slot_1, true, [] {
                player::modify_player_skill(1, g_settings->skill.slot_1);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 2", nullptr, &skill_list_v, &g_settings->skill.slot_2, true, [] {
                player::modify_player_skill(2, g_settings->skill.slot_2);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 3", nullptr, &skill_list_v, &g_settings->skill.slot_3, true, [] {
                player::modify_player_skill(3, g_settings->skill.slot_3);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 4", nullptr, &skill_list_v, &g_settings->skill.slot_4, true, [] {
                player::modify_player_skill(4, g_settings->skill.slot_4);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 5", nullptr, &skill_list_v, &g_settings->skill.slot_5, true, [] {
                player::modify_player_skill(5, g_settings->skill.slot_5);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 6", nullptr, &skill_list_v, &g_settings->skill.slot_6, true, [] {
                player::modify_player_skill(6, g_settings->skill.slot_6);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 7", nullptr, &skill_list_v, &g_settings->skill.slot_7, true, [] {
                player::modify_player_skill(7, g_settings->skill.slot_7);
            });
            sub->add_option<choose_option<std::string, int>>("Skill Slot 8", nullptr, &skill_list_v, &g_settings->skill.slot_8, true, [] {
                player::modify_player_skill(8, g_settings->skill.slot_8);
            });
            // sub->add_option<choose_option<std::uint64_t, std::size_t>>("Vector", nullptr, &vector, &vectorPos);
        });
    }
}