#pragma once

#include "base_option.hpp"
#include "utility/utility.hpp"

namespace big
{
	template <typename DataType, typename PositionType>
	class choose_option : public base_option<choose_option<DataType, PositionType>>
	{
	public:
		explicit choose_option() = default;

		template <PositionType N>
		explicit choose_option(const char* text, const char* description, DataType(*array)[N], PositionType* position, bool actionOnHorizontal = false, std::function<void()> action = [] {}) :
			m_data(&(*array)[0]),
			m_data_size(N),
			m_position(position),
			m_action_on_horizontal(actionOnHorizontal)
		{
			Base::set_left_text(text);
			if (description)
				Base::set_description(description);
			Base::set_action(std::move(action));
		}

		template <typename VectorType>
		explicit choose_option(const char* text, const char* description, const VectorType* v, PositionType* position, bool actionOnHorizontal = false, std::function<void()> action = [] {}) :
			m_data(v->data()),
			m_data_size(v->size()),
			m_position(position),
			m_action_on_horizontal(actionOnHorizontal)
		{
			Base::set_left_text(text);
			if (description)
				Base::set_description(description);
			Base::set_action(std::move(action));
		}

		const char* get_right_text() override
		{
			MemoryStringStream stream(Base::m_right_text);

			if (m_data)
			{
				stream << m_data[*m_position];
				stream << " [" << *m_position + 1 << " / " << m_data_size << "]";
			}
			else
			{
				stream << "Unknown [0 / 0]";
			}

			return Base::get_right_text();
		}

		void handle_action(OptionAction action) override
		{
			if (action == OptionAction::LeftPress)
			{
				if (m_data)
				{
					if (*m_position > 0)
						--(*m_position);
					else
						*m_position = static_cast<PositionType>(m_data_size - 1);

					if (m_action_on_horizontal && Base::m_action)
						std::invoke(Base::m_action);
				}
			}
			else if (action == OptionAction::RightPress)
			{
				if (m_data)
				{
					if (*m_position < m_data_size - 1)
						++(*m_position);
					else
						*m_position = 0;

					if (m_action_on_horizontal && Base::m_action)
						std::invoke(Base::m_action);
				}
			}

			if (m_data)
				Base::handle_action(action);
		}

		~choose_option() noexcept = default;
		choose_option(choose_option const&) = default;
		choose_option& operator=(choose_option const&) = default;
		choose_option(choose_option&&) = default;
		choose_option& operator=(choose_option&&) = default;
	private:
		const DataType* m_data{};
		PositionType m_data_size{};
		PositionType* m_position{};
		bool m_action_on_horizontal{};

		using Base = base_option<choose_option<DataType, PositionType>>;
	};
}
