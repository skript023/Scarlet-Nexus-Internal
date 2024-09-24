#pragma once
#include "base_option.hpp"
#include "ui_manager.hpp"

namespace big
{
	template <typename BoolType = bool>
	class bool_option : public base_option<bool_option<BoolType>>
	{
	public:
		explicit bool_option(const char* text, const char* description, BoolType* b00l, std::function<void()> action = [] {}) :
			m_bool(b00l)
		{
			Base::set_left_text(text);
			if (description)
				Base::set_description(description);
			Base::set_action(std::move(action));
		}

		~bool_option() noexcept = default;
		bool_option(bool_option const&) = default;
		bool_option& operator=(bool_option const&) = default;
		bool_option(bool_option&&) = default;
		bool_option& operator=(bool_option&&) = default;

		void handle_action(OptionAction action) override
		{
			if (action == OptionAction::EnterPress)
			{
				*m_bool = !*m_bool;
			}

			Base::handle_action(action);
		}

		bool get_flag(OptionFlag flag) override
		{
			if (flag == OptionFlag::Toggle)
			{
				g_ui_manager->m_bool_option = *m_bool;
				return true;
			}

			return Base::get_flag(flag);
		}
	private:
		BoolType* m_bool;
		using Base = base_option<bool_option<BoolType>>;
	};
}
