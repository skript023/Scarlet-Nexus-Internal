#pragma once
#include "common.hpp"
#include "abstract_option.hpp"

namespace big
{
	template <typename T>
	class base_option : public abstract_option
	{
	public:
		const char* get_left_text() override
		{
			return &m_left_text[0];
		}

		const char* get_right_text() override
		{
			return &m_right_text[0];
		}

		const char* get_description() override
		{
			return &m_description[0];
		}

		void handle_action(OptionAction action) override
		{
			if (action == OptionAction::EnterPress)
			{
				if (m_action)
				{
					std::invoke(m_action);
				}
			}
		}

		bool get_flag(OptionFlag) override
		{
			return false;
		}

		T& set_left_text(const char* text)
		{
			std::strncpy(&m_left_text[0], text, sizeof(m_left_text) - 1);
			return static_cast<T&>(*this);
		}

		T& set_right_text(const char* text)
		{
			std::strncpy(&m_right_text[0], text, sizeof(m_right_text) - 1);
			return static_cast<T&>(*this);
		}

		T& set_description(const char* text)
		{
			if (text)
				std::strncpy(&m_description[0], text, sizeof(m_description) - 1);
			return static_cast<T&>(*this);
		}

		T& set_action(std::function<void()> action)
		{
			m_action = std::move(action);
			return static_cast<T&>(*this);
		}
	protected:
		explicit base_option() = default;
		~base_option() noexcept = default;

		base_option(base_option const&) = default;
		base_option& operator=(base_option const&) = default;
		base_option(base_option&&) = default;
		base_option& operator=(base_option&&) = default;

		char m_left_text[64] = {};
		char m_right_text[64] = {};
		char m_description[64] = {};
		std::function<void()> m_action;
	};
}
