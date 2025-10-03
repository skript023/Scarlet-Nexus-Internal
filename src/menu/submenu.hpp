#pragma once

#include <cstdint>
#include "ui/canvas.hpp"
#include "ui/tabs_menu.hpp"
#include "ui/bool_option.hpp"
#include "ui/choose_option.hpp"
#include "ui/number_option.hpp"
#include "ui/reguler_option.hpp"
#include "ui/sub_option.hpp"
#include "ui/reguler_submenu.hpp"
#include "ui/player_submenu.hpp"
#include "ui/bool_slider_int_option.hpp"
#include "ui/bool_slider_float_option.hpp"

namespace big
{
	enum Submenu : std::uint32_t
	{
		SubmenuHome,
		SubmenuPlayer,
		SubmenuStats,
		SubmenuESP,
		SubmenuMovement,
		SubmenuTeleport,
		SubmenuCustomTeleport,
		SubmenuTest,
		SubmenuPlayerList,
		SubmenuSettings,
		SubmenuSettingsSubmenuBar,
		SubmenuSettingsOption,
		SubmenuSettingsFooter,
		SubmenuSettingsInput,
		SubmenuSelectedPlayer
	};
}