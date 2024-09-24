#pragma once
#include "base_submenu.hpp"

namespace big
{
	class regular_submenu : public base_submenu<regular_submenu>
	{
	public:
		explicit regular_submenu() = default;
		~regular_submenu() noexcept = default;
		explicit regular_submenu(const char* name, std::uint32_t id, std::function<void(regular_submenu*)> action)
		{
			set_name(name);
			set_id(id);
			set_action(std::move(action));
		}

		regular_submenu(regular_submenu const&) = default;
		regular_submenu& operator=(regular_submenu const&) = default;
		regular_submenu(regular_submenu&&) = default;
		regular_submenu& operator=(regular_submenu&&) = default;
	};
}