#pragma once
#include "base_option.hpp"
#include "canvas.hpp"

namespace big
{
	class sub_option : public base_option<sub_option>
	{
	public:
		explicit sub_option() = default;
		explicit sub_option(const char* text, const char* description, std::uint32_t subId, std::function<void()> action = [] {}) :
			m_sub_id(subId)
		{
			set_left_text(text);
			if (description)
				set_description(description);
			set_action(std::move(action));
		}

		~sub_option() noexcept = default;
		sub_option(sub_option const&) = default;
		sub_option& operator=(sub_option const&) = default;
		sub_option(sub_option&&) = default;
		sub_option& operator=(sub_option&&) = default;

		void handle_action(OptionAction action) override
		{
			if (action == OptionAction::EnterPress)
			{
				canvas::switch_to_submenu(m_sub_id);
			}

			base_option::handle_action(action);
		}

		bool get_flag(OptionFlag flag) override
		{
			if (flag == OptionFlag::Enterable)
			{
				return true;
			}

			return base_option::get_flag(flag);
		}
	private:
		std::uint32_t m_sub_id{};
	};
}