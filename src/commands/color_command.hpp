#pragma once
#include "Command.hpp"
#include "imgui.h"

namespace big
{
	class color_command : public command
	{
	protected:
		virtual void on_change(){};
		virtual void on_call() override;
		virtual void save_state(nlohmann::json& value) override;
		virtual void load_state(nlohmann::json& value) override;

		ImVec4 m_color_state = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	public:
		color_command(std::string name, std::string label, std::string description, ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		ImVec4 get_state();
		void set_color_state(ImVec4 state);
	};
}