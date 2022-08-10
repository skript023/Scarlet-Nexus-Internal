#include "imgui.h"
#include <script.hpp>
#include "setting_menu.h"
#include "utility/player.hpp"
#include "settings.hpp"

namespace big
{
    void setting_menu::render_menu()
    {
        if (ImGui::BeginTabItem("Setting"))
        {
            if (ImGui::Button("Quit"))
            {
                g_running = false;
            }

            ImGui::EndTabItem();
        }
    }
}