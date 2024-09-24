#pragma once
#include "abstract_submenu.hpp"
#include "abstract_option.hpp"

namespace big
{
	template <typename T>
	class base_submenu : public abstract_submenu
	{
	public:
		const char* get_name() override
		{
			return &m_name[0];
		}

		std::uint32_t get_id() override
		{
			return m_id;
		}

		void execute() override
		{
			if (m_action)
				std::invoke(m_action, static_cast<T*>(this));
			if (m_selected_option >= m_options.size())
				m_selected_option = 0;
		}

		void reset() override
		{
			m_options.clear();
		}

		abstract_option* get_option(std::size_t i) override
		{
			if (i > m_options.size())
				return nullptr;
			return m_options[i].get();
		}

		std::size_t get_num_option() override
		{
			return m_options.size();
		}

		std::size_t get_selected_option() override
		{
			return m_selected_option;
		}

		void set_selected_option(std::size_t n) override
		{
			m_selected_option = n;
		}

		template <typename OptionType, typename ...TArgs>
		void add_option(TArgs&&... args)
		{
			m_options.push_back(std::make_unique<OptionType>(std::forward<TArgs>(args)...));
		}

		T& set_name(const char* name)
		{
			std::strncpy(&m_name[0], name, sizeof(m_name) - 1);
			return static_cast<T&>(*this);
		}

		T& set_id(std::uint32_t id)
		{
			m_id = id;
			return static_cast<T&>(*this);
		}

		T& set_action(std::function<void(T*)> action)
		{
			m_action = std::move(action);
			return static_cast<T&>(*this);
		}
	protected:
		explicit base_submenu() = default;
		~base_submenu() noexcept = default;

		base_submenu(base_submenu const&) = default;
		base_submenu& operator=(base_submenu const&) = default;
		base_submenu(base_submenu&&) = default;
		base_submenu& operator=(base_submenu&&) = default;

		char m_name[64] = {};
		std::uint32_t m_id{};
		std::function<void(T*)> m_action;

		std::vector<std::unique_ptr<abstract_option>> m_options;
		std::size_t m_selected_option{};
	};
}
