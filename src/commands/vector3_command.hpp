#pragma once
#include "command.hpp"
#include <class/vector.hpp>

namespace big
{
	class vector3_command : public command
	{
	protected:
		virtual void on_change(){};
		virtual void on_call() override;
		virtual void save_state(nlohmann::json& value) override;
		virtual void load_state(nlohmann::json& value) override;

		Vector3 m_state{};

	public:
		vector3_command(std::string name, std::string label, std::string description, Vector3 def_val = {});
		Vector3 get_state();
		void set_state(const Vector3& state);
	};
}