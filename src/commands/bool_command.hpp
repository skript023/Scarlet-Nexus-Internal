#pragma once
#include "Command.hpp"

namespace big
{
	class bool_command : public command
	{
	protected:
		virtual void on_enable() {};
		virtual void on_disable() {};
		virtual void on_call() override;
		virtual void save_state(nlohmann::json& value) override;
		virtual void load_state(nlohmann::json& value) override;

		bool m_state = false;
	public:
		bool_command(std::string name, std::string label, std::string description, bool def_value = false);
		bool& get_state();
		void set_state(bool state);
		void initialize();
		void shutdown();
	};
}