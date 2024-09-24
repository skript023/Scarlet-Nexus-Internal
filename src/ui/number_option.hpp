#pragma once
#include "base_option.hpp"
#include "ui_manager.hpp"
#include "utility/utility.hpp"

namespace big
{
	template <typename NumberType>
	class number_option : public base_option<number_option<NumberType>>
	{
	public:
		explicit number_option() = default;
		explicit number_option(const char* text, const char* description, NumberType* number, NumberType min, NumberType max, NumberType step = 1, std::size_t precision = 3, bool actionOnHorizontal = true, const char* prefix = "", const char* suffix = "", std::function<void()> action = [] {}) :
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
			std::strncpy(&m_prefix[0], prefix, sizeof(m_prefix) - 1);
			std::strncpy(&m_suffix[0], suffix, sizeof(m_suffix) - 1);
		}

		~number_option() noexcept = default;
		number_option(number_option const&) = default;
		number_option& operator=(number_option const&) = default;
		number_option(number_option&&) = default;
		number_option& operator=(number_option&&) = default;

		const char* get_right_text() override
		{
			MemoryStringStream stream(Base::m_right_text);
			stream
				<< std::setprecision(m_precision)
				<< std::fixed
				<< m_prefix
				<< static_cast<DisplayType>(*m_number)
				<< m_suffix;
			return Base::get_right_text();
		}

		void handle_action(OptionAction action) override
		{
			if (action == OptionAction::LeftPress)
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
	private:
		char m_prefix[32] = {};
		char m_suffix[32] = {};
		bool m_action_on_horizontal{};

		NumberType* m_number{};
		NumberType m_min{};
		NumberType m_max{};
		NumberType m_step{};
		std::size_t m_precision{};


		using Base = base_option<number_option<NumberType>>;
		using DisplayType = std::conditional_t<sizeof(NumberType) == 1, std::uint32_t, NumberType>;
	};
}
