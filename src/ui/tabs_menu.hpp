#pragma once
#include "base_submenu.hpp"

namespace big
{
	class tabs_menu : public base_submenu<tabs_menu>
	{
    public:
		explicit tabs_menu() = default;
		~tabs_menu() noexcept = default;
		explicit tabs_menu(const char* name, std::uint32_t id, std::function<void(tabs_menu*)> action)
		{
			set_name(name);
			set_id(id);
			set_action(std::move(action));
		}

		tabs_menu(tabs_menu const&) = default;
		tabs_menu& operator=(tabs_menu const&) = default;
		tabs_menu(tabs_menu&&) = default;
		tabs_menu& operator=(tabs_menu&&) = default;
	};
}