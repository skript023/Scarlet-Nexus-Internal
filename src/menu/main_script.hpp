#pragma once
#include "ui/ui_manager.hpp"
#include "ui/bool_option.hpp"
#include "ui/choose_option.hpp"
#include "ui/number_option.hpp"
#include "ui/reguler_option.hpp"
#include "ui/sub_option.hpp"
#include "ui/reguler_submenu.hpp"
#include "ui/player_submenu.hpp"

namespace big::main_script
{
    enum Submenu : std::uint32_t
	{
		SubmenuHome,
		SubmenuLocal,
		SubmenuMovement,
		SubmenuTest,
		SubmenuPlayerList,
		SubmenuSettings,
		SubmenuSettingsSubmenuBar,
		SubmenuSettingsOption,
		SubmenuSettingsFooter,
		SubmenuSettingsInput,
		SubmenuSelectedPlayer
	};

    inline void initialize_main()
    {
        LOG(INFO) << "Main Script Initialization";

        g_ui_manager->add_submenu<regular_submenu>("Home", SubmenuHome, [](regular_submenu* sub)
		{
			sub->add_option<sub_option>("Local", nullptr, SubmenuLocal);
			sub->add_option<sub_option>("Weapon", nullptr, SubmenuTest);
			sub->add_option<sub_option>("Vehicle", nullptr, SubmenuTest);
			sub->add_option<sub_option>("Network", nullptr, SubmenuTest);
			sub->add_option<sub_option>("Playerlist", nullptr, SubmenuPlayerList);
			sub->add_option<sub_option>("Miscellaneous", nullptr, SubmenuTest);
			sub->add_option<sub_option>("World", nullptr, SubmenuTest);
			sub->add_option<sub_option>("Teleport", nullptr, SubmenuTest);
			sub->add_option<sub_option>("Recovery", nullptr, SubmenuTest);
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

        g_ui_manager->add_submenu<regular_submenu>("Settings", SubmenuSettings, [](regular_submenu* sub)
        {
            sub->add_option<sub_option>("Infobar", nullptr, SubmenuSettingsSubmenuBar);
            sub->add_option<sub_option>("Options", nullptr, SubmenuSettingsOption);
            sub->add_option<sub_option>("Footer", nullptr, SubmenuSettingsFooter);
            sub->add_option<sub_option>("Input", nullptr, SubmenuSettingsInput);
            sub->add_option<number_option<float>>("X Position", nullptr, &g_ui_manager->m_pos.x, 0.f, 2000.f, 25.f, 2);
            sub->add_option<number_option<float>>("Y Position", nullptr, &g_ui_manager->m_pos.y, 0.f, 2000.f, 25.f, 2);
            sub->add_option<number_option<float>>("Width", nullptr, &g_ui_manager->m_width, 0.f, 1000.f, 50.f, 2);
            sub->add_option<bool_option<bool>>("Sounds", nullptr, &g_ui_manager->m_sounds);
            sub->add_option<reguler_option>("Unload", nullptr, [] { g_running = false; });
            sub->add_option<reguler_option>("Exit", nullptr, [] { exit(0); });
        });

        g_ui_manager->add_submenu<regular_submenu>("Infobar", SubmenuSettingsSubmenuBar, [](regular_submenu* sub)
        {
            sub->add_option<number_option<std::uint8_t>>("Background R", nullptr, &g_ui_manager->m_submenu_bar_background_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Background G", nullptr, &g_ui_manager->m_submenu_bar_background_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Background B", nullptr, &g_ui_manager->m_submenu_bar_background_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Background A", nullptr, &g_ui_manager->m_submenu_bar_background_color.a, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Text R", nullptr, &g_ui_manager->m_submenu_bar_text_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Text G", nullptr, &g_ui_manager->m_submenu_bar_text_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Text B", nullptr, &g_ui_manager->m_submenu_bar_text_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Text A", nullptr, &g_ui_manager->m_submenu_bar_text_color.a, '\0', static_cast<std::uint8_t>(255));
        });

        g_ui_manager->add_submenu<regular_submenu>("Options", SubmenuSettingsOption, [](regular_submenu* sub)
        {
            sub->add_option<number_option<std::uint8_t>>("Selected Background R", nullptr, &g_ui_manager->m_option_selected_background_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Selected Background G", nullptr, &g_ui_manager->m_option_selected_background_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Selected Background B", nullptr, &g_ui_manager->m_option_selected_background_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Selected Background A", nullptr, &g_ui_manager->m_option_selected_background_color.a, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Background R", nullptr, &g_ui_manager->m_option_unselected_background_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Background G", nullptr, &g_ui_manager->m_option_unselected_background_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Background B", nullptr, &g_ui_manager->m_option_unselected_background_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Background A", nullptr, &g_ui_manager->m_option_unselected_background_color.a, '\0', static_cast<std::uint8_t>(255));

            sub->add_option<number_option<std::uint8_t>>("Selected Text R", nullptr, &g_ui_manager->m_option_selected_background_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Selected Text G", nullptr, &g_ui_manager->m_option_selected_background_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Selected Text B", nullptr, &g_ui_manager->m_option_selected_background_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Selected Text A", nullptr, &g_ui_manager->m_option_selected_background_color.a, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Text R", nullptr, &g_ui_manager->m_option_unselected_text_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Text G", nullptr, &g_ui_manager->m_option_unselected_text_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Text B", nullptr, &g_ui_manager->m_option_unselected_text_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Text A", nullptr, &g_ui_manager->m_option_unselected_text_color.a, '\0', static_cast<std::uint8_t>(255));
        });

        g_ui_manager->add_submenu<regular_submenu>("Footer", SubmenuSettingsFooter, [](regular_submenu* sub)
        {
            sub->add_option<number_option<std::uint8_t>>("Background R", nullptr, &g_ui_manager->m_FooterBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Background G", nullptr, &g_ui_manager->m_FooterBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Background B", nullptr, &g_ui_manager->m_FooterBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Background A", nullptr, &g_ui_manager->m_FooterBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
        });

        g_ui_manager->add_submenu<regular_submenu>("Input", SubmenuSettingsInput, [](regular_submenu* sub)
        {
            sub->add_option<number_option<std::int32_t>>("Open Delay", nullptr, &g_ui_manager->m_open_delay, 10, 1000, 10, 0);
            sub->add_option<number_option<std::int32_t>>("Back Delay", nullptr, &g_ui_manager->m_back_delay, 10, 1000, 10, 0);
            sub->add_option<number_option<std::int32_t>>("Enter Delay", nullptr, &g_ui_manager->m_enter_delay, 10, 1000, 10, 0);
            sub->add_option<number_option<std::int32_t>>("Vertical Delay", nullptr, &g_ui_manager->m_vectical_delay, 10, 1000, 10, 0);
            sub->add_option<number_option<std::int32_t>>("Horizontal Delay", nullptr, &g_ui_manager->m_horizontal_delay, 10, 1000, 10, 0);
        });
    }
}