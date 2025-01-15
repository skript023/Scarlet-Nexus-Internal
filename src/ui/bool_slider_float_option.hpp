#pragma once
#include "base_option.hpp"
#include "ui_manager.hpp"

namespace big
{
	class bool_slider_float_option : public base_option<bool_slider_float_option>
	{
	public:
		explicit bool_slider_float_option(const char* text, const char* description, bool* b00l, float* number, float min, float max, float step = 1.f, std::size_t precision = 3, bool actionOnHorizontal = true, std::function<void()> action = [] {}) :
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

		~bool_slider_float_option() noexcept = default;
		bool_slider_float_option(bool_slider_float_option const&) = default;
		bool_slider_float_option& operator=(bool_slider_float_option const&) = default;
		bool_slider_float_option(bool_slider_float_option&&) = default;
		bool_slider_float_option& operator=(bool_slider_float_option&&) = default;

		float get_float() override
		{
			return *m_number;
		}

		float get_min_float() override
		{
			return m_min;
		}

		float get_max_float() override
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
			if (flag == OptionFlag::BoolSliderFloat)
			{
				g_ui_manager.m_bool_slider_float_option = *m_bool;

				return true;
			}

			return Base::get_flag(flag);
		}
	private:
		bool* m_bool;
		bool m_action_on_horizontal{};
		float* m_number{};
		float m_min{};
		float m_max{};
		float m_step{};
		std::size_t m_precision{};
		using Base = base_option<bool_slider_float_option>;
	};
}
