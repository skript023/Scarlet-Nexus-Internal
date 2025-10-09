#pragma once
#include "command.hpp"

namespace big
{
	class list_command : public command
	{
	protected:
		virtual void on_change(){};
		virtual void on_call() override;
		virtual void save_state(nlohmann::json& value) override;
		virtual void load_state(nlohmann::json& value) override;

		int m_state = 0;
		std::vector<std::pair<int, const char*>> m_list; // not very efficient but keeps things sorted

	public:
		list_command(std::string name, std::string label, std::string description, std::vector<std::pair<int, const char*>> list, int def_val = 0);
		int get_state();
		void set_state(int state);
		std::vector<std::pair<int, const char*>>& get_list();
	};
}