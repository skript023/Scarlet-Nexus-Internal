#pragma once

#include <cstdint>
#include "ui/ui_manager.hpp"
#include "ui/bool_option.hpp"
#include "ui/choose_option.hpp"
#include "ui/number_option.hpp"
#include "ui/reguler_option.hpp"
#include "ui/sub_option.hpp"
#include "ui/reguler_submenu.hpp"
#include "ui/player_submenu.hpp"

namespace big
{
    enum Submenu : std::uint32_t
	{
		SubmenuHome,
		SubmenuPlayer,
		SubmenuStats,
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
}