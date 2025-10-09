#pragma once
#include "command.hpp"

namespace big
{
	class float_command : public command
	{
	protected:
		virtual void on_change(){};
		virtual void on_call() override;
		virtual void save_state(nlohmann::json& value) override;
		virtual void load_state(nlohmann::json& value) override;

		float m_state = 0;
		float m_min;
		float m_max;

	public:
		float_command(std::string name, std::string label, std::string description, float min, float max, float def_val = 0.0f);
		float& get_state();
		void set_state(float state);
		float get_minimum();
		float get_maximum();
	};
}