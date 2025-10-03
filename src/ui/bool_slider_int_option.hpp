#pragma once
#include "base_option.hpp"
#include "canvas.hpp"

namespace big
{
	class bool_slider_int_option : public base_option<bool_slider_int_option>
	{
	public:
		explicit bool_slider_int_option(const char* text, const char* description, bool* b00l, int* number, int min, int max, int step = 1, std::size_t precision = 3, bool actionOnHorizontal = true, std::function<void()> action = [] {}) :
			m_bool(b00l),
			m_action_on_horizontal(actionOnHorizontal),
			m_number(number),
			m_min(min),
			m_max(max),
			m_step(step),
			m_precision(precision)
		{
			Base::set_left_text(text);
			if (description)
				Base::set_description(description);
			Base::set_action(std::move(action));
		}

		~bool_slider_int_option() noexcept = default;
		bool_slider_int_option(bool_slider_int_option const&) = default;
		bool_slider_int_option& operator=(bool_slider_int_option const&) = default;
		bool_slider_int_option(bool_slider_int_option&&) = default;
		bool_slider_int_option& operator=(bool_slider_int_option&&) = default;

		int get_integer() override
		{
			return *m_number;
		}

		int get_min_integer() override
		{
			return m_min;
		}

		int get_max_integer() override
		{
			return m_max;
		}

		void handle_action(OptionAction action) override
		{
			if (action == OptionAction::EnterPress)
			{
				*m_bool = !*m_bool;
			}
			else if (action == OptionAction::LeftPress)
			{
				if (*m_number - m_step < m_min)
					*m_number = m_max;
				else
					*m_number -= m_step;

				if (m_action_on_horizontal && Base::m_action)
					std::invoke(Base::m_action);
			}
			else if (action == OptionAction::RightPress)
			{
				if (*m_number + m_step > m_max)
					*m_number = m_min;
				else
					*m_number += m_step;

				if (m_action_on_horizontal && Base::m_action)
					std::invoke(Base::m_action);
			}

			Base::handle_action(action);
		}

		bool get_flag(OptionFlag flag) override
		{
			if (flag == OptionFlag::BoolSliderInt)
			{
				canvas::set_bool_slider_int(m_bool);

				return true;
			}

			return Base::get_flag(flag);
		}
	private:
		bool* m_bool;
		bool m_action_on_horizontal{};
		int* m_number{};
		int m_min{};
		int m_max{};
		int m_step{};
		std::size_t m_precision{};
		using Base = base_option<bool_slider_int_option>;
	};
}
