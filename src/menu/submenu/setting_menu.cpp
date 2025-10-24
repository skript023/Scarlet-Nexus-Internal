#include "../view.hpp"
#include "script.hpp"
#include "utility/player.hpp"
#include "server/server_module.hpp"

namespace big
{
    void view::setting_submenu()
    {
        canvas::add_submenu<regular_submenu>("Settings", SubmenuSettings, [](regular_submenu* sub)
        {
            sub->add_option<sub_option>("Infobar", nullptr, SubmenuSettingsSubmenuBar);
            sub->add_option<sub_option>("Options", nullptr, SubmenuSettingsOption);
            sub->add_option<sub_option>("Footer", nullptr, SubmenuSettingsFooter);
            sub->add_option<sub_option>("Input", nullptr, SubmenuSettingsInput);
            sub->add_option<number_option<float>>("X Position", nullptr, &g_settings.window.m_pos.x, 0.f, 2000.f, 25.f, 2);
            sub->add_option<number_option<float>>("Y Position", nullptr, &g_settings.window.m_pos.y, 0.f, 2000.f, 25.f, 2);
            sub->add_option<number_option<float>>("Width", nullptr, &g_settings.window.m_width, 0.f, 1000.f, 50.f, 2);
            sub->add_option<bool_option<bool>>("Sounds", nullptr, &g_settings.window.m_sounds);
            sub->add_option<bool_option<bool>>("Draw Mouse", nullptr, &g_settings.window.mouse_active);
            sub->add_option<reguler_option>("Unload", nullptr, [] { g_running = false; });
            sub->add_option<reguler_option>("Exit", nullptr, [] { exit(0); });
        });

        canvas::add_submenu<regular_submenu>("Infobar", SubmenuSettingsSubmenuBar, [](regular_submenu* sub)
        {
            sub->add_option<number_option<std::uint8_t>>("Background R", nullptr, &g_settings.window.m_submenu_bar_background_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Background G", nullptr, &g_settings.window.m_submenu_bar_background_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Background B", nullptr, &g_settings.window.m_submenu_bar_background_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Background A", nullptr, &g_settings.window.m_submenu_bar_background_color.a, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Text R", nullptr, &g_settings.window.m_submenu_bar_text_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Text G", nullptr, &g_settings.window.m_submenu_bar_text_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Text B", nullptr, &g_settings.window.m_submenu_bar_text_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Text A", nullptr, &g_settings.window.m_submenu_bar_text_color.a, '\0', static_cast<std::uint8_t>(255));
        });

        canvas::add_submenu<regular_submenu>("Options", SubmenuSettingsOption, [](regular_submenu* sub)
        {
            sub->add_option<number_option<std::uint8_t>>("Selected Background R", nullptr, &g_settings.window.m_option_selected_background_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Selected Background G", nullptr, &g_settings.window.m_option_selected_background_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Selected Background B", nullptr, &g_settings.window.m_option_selected_background_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Selected Background A", nullptr, &g_settings.window.m_option_selected_background_color.a, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Background R", nullptr, &g_settings.window.m_option_unselected_background_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Background G", nullptr, &g_settings.window.m_option_unselected_background_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Background B", nullptr, &g_settings.window.m_option_unselected_background_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Background A", nullptr, &g_settings.window.m_option_unselected_background_color.a, '\0', static_cast<std::uint8_t>(255));

            sub->add_option<number_option<std::uint8_t>>("Selected Text R", nullptr, &g_settings.window.m_option_selected_background_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Selected Text G", nullptr, &g_settings.window.m_option_selected_background_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Selected Text B", nullptr, &g_settings.window.m_option_selected_background_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Selected Text A", nullptr, &g_settings.window.m_option_selected_background_color.a, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Text R", nullptr, &g_settings.window.m_option_unselected_text_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Text G", nullptr, &g_settings.window.m_option_unselected_text_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Text B", nullptr, &g_settings.window.m_option_unselected_text_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Unselected Text A", nullptr, &g_settings.window.m_option_unselected_text_color.a, '\0', static_cast<std::uint8_t>(255));
        });

        canvas::add_submenu<regular_submenu>("Footer", SubmenuSettingsFooter, [](regular_submenu* sub)
        {
            sub->add_option<number_option<std::uint8_t>>("Background R", nullptr, &g_settings.window.m_footer_background_color.r, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Background G", nullptr, &g_settings.window.m_footer_background_color.g, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Background B", nullptr, &g_settings.window.m_footer_background_color.b, '\0', static_cast<std::uint8_t>(255));
            sub->add_option<number_option<std::uint8_t>>("Background A", nullptr, &g_settings.window.m_footer_background_color.a, '\0', static_cast<std::uint8_t>(255));
        });

        canvas::add_submenu<regular_submenu>("Input", SubmenuSettingsInput, [](regular_submenu* sub)
        {
            sub->add_option<number_option<std::int32_t>>("Open Delay", nullptr, &g_settings.window.m_open_delay, 10, 1000, 10, 0);
            sub->add_option<number_option<std::int32_t>>("Back Delay", nullptr, &g_settings.window.m_back_delay, 10, 1000, 10, 0);
            sub->add_option<number_option<std::int32_t>>("Enter Delay", nullptr, &g_settings.window.m_enter_delay, 10, 1000, 10, 0);
            sub->add_option<number_option<std::int32_t>>("Vertical Delay", nullptr, &g_settings.window.m_vectical_delay, 10, 1000, 10, 0);
            sub->add_option<number_option<std::int32_t>>("Horizontal Delay", nullptr, &g_settings.window.m_horizontal_delay, 10, 1000, 10, 0);
        });
    }
}