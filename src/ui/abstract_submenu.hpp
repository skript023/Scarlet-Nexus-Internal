#pragma once
#include "abstract_option.hpp"

namespace big
{
	class abstract_submenu
	{
	public:
		virtual ~abstract_submenu() noexcept = default;

		virtual const char* get_name() = 0;
		virtual std::uint32_t get_id() = 0;
		virtual void execute() = 0;
		virtual void reset() = 0;

		virtual abstract_option* get_option(std::size_t i) = 0;
		virtual std::size_t get_num_option() = 0;

		virtual std::size_t get_selected_option() = 0;
		virtual void set_selected_option(std::size_t) = 0;

		void ScrollForward()
		{
			if (get_selected_option() < get_num_option() - 1)
				set_selected_option(get_selected_option() + 1);
			else
				set_selected_option(0);
		}

		void ScrollBackward()
		{
			if (get_selected_option() > 0)
				set_selected_option(get_selected_option() - 1);
			else
				set_selected_option(get_selected_option() - 1);
		}
	protected:
		explicit abstract_submenu() = default;

		abstract_submenu(abstract_submenu const&) = default;
		abstract_submenu& operator=(abstract_submenu const&) = default;

		abstract_submenu(abstract_submenu&&) = default;
		abstract_submenu& operator=(abstract_submenu&&) = default;
	};
}