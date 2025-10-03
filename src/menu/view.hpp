#pragma once
#include "imgui.h"
#include "submenu.hpp"

namespace big
{
	class view
	{
	public:
		static void register_submenu()
		{
			home();
			player_submenu();
			stats_submenu();
			//teleport_submenu();
			//esp_submenu();
			setting_submenu();
		}
	public:
		static void home();
		static void draw_input();
		static void draw_overlay();
		static void notifications();
	public:
		//static void esp_submenu();
		static void player_submenu();
		static void stats_submenu();
		//static void teleport_submenu();
		static void setting_submenu();
	};
}