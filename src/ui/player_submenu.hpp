#pragma once
#include "pointers.hpp"
#include "base_submenu.hpp"
#include "utility/utility.hpp"

namespace big
{
	class player_submenu : public base_submenu<player_submenu>
	{
	public:
		explicit player_submenu() = default;
		~player_submenu() noexcept = default;
		explicit player_submenu(std::uint32_t* selectedPlayer, std::uint32_t id, std::function<void(player_submenu*)> action) :
			m_selected_player(selectedPlayer)
		{
			set_id(id);
			set_action(std::move(action));
		}

		player_submenu(player_submenu const&) = default;
		player_submenu& operator=(player_submenu const&) = default;
		player_submenu(player_submenu&&) = default;
		player_submenu& operator=(player_submenu&&) = default;

		const char* get_name() override
		{
			// return g_pointers->m_get_player_name(*m_selected_player);
		}
	private:
		std::uint32_t* m_selected_player;
	};
}