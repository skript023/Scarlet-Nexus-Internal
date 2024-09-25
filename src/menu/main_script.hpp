#pragma once
#include "submenu.hpp"
#include "player_menu.h"
#include "player_stats.h"
#include "setting_menu.h"
namespace big
{
    class main_script
    {
    public:
        static void initialize_main()
        {
            render_home();
            player_menu::render_submenu();
            setting_menu::render_submenu();
            player_stat::render_submenu();
        }
        static void render_home()
        {
            g_ui_manager->add_submenu<regular_submenu>("Home", SubmenuHome, [](regular_submenu* sub)
            {
                sub->add_option<sub_option>("Player", nullptr, SubmenuPlayer);
                sub->add_option<sub_option>("Stats", nullptr, SubmenuStats);
                // sub->add_option<sub_option>("Vehicle", nullptr, SubmenuTest);
                // sub->add_option<sub_option>("Network", nullptr, SubmenuTest);
                // sub->add_option<sub_option>("Playerlist", nullptr, SubmenuPlayerList);
                // sub->add_option<sub_option>("Miscellaneous", nullptr, SubmenuTest);
                // sub->add_option<sub_option>("World", nullptr, SubmenuTest);
                // sub->add_option<sub_option>("Teleport", nullptr, SubmenuTest);
                // sub->add_option<sub_option>("Recovery", nullptr, SubmenuTest);
                sub->add_option<sub_option>("Settings", nullptr, SubmenuSettings);
            });

            g_ui_manager->add_submenu<regular_submenu>("Demo", SubmenuTest, [](regular_submenu* sub)
            {
                sub->add_option<reguler_option>("Option", nullptr, []
                {
                    LOG(INFO) << "You pressed the test option";
                });

                static bool testBool1{};
                sub->add_option<bool_option<bool>>("Bool", nullptr, &testBool1);

                static std::int32_t int32Test{ 69 };
                sub->add_option<number_option<std::int32_t>>("Int32", nullptr, &int32Test, 0, 100);

                static std::int64_t int64Test{ 420 };
                sub->add_option<number_option<std::int64_t>>("Int64", nullptr, &int64Test, 0, 1000, 10);

                static float floatTest{ 6.9f };
                sub->add_option<number_option<float>>("Float", nullptr, &floatTest, 0.f, 10.f, 0.1f, 1);

                static std::vector<std::uint64_t> vector{ 1, 2, 3 };
                static std::size_t vectorPos{};

                // sub->add_option<choose_option<const char*, std::size_t>>("Array", nullptr, &Lists::DemoList, &Lists::DemoListPos);
                sub->add_option<choose_option<std::uint64_t, std::size_t>>("Vector", nullptr, &vector, &vectorPos);
            });
        }
    };
}