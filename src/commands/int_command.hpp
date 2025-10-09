#pragma once
#include "Command.hpp"

namespace big
{
	class int_command : public command
	{
	protected:
		virtual void on_change(){};
		virtual void on_call() override;
		virtual void save_state(nlohmann::json& value) override;
		virtual void load_state(nlohmann::json& value) override;

		int m_state = 0;
		std::optional<int> m_min;
		std::optional<int> m_max;

	public:
		int_command(std::string name, std::string label, std::string description, std::optional<int> min = std::nullopt, std::optional<int> max = std::nullopt, int def_val = 0);
		int get_state();
		void set_state(int state);
		std::optional<int> get_minimum();
		std::optional<int> get_maximum();
	};
}