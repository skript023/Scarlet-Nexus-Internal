#include "vector3_command.hpp"
#include "fiber_pool.hpp" // TODO: game import in core

namespace big
{
	void vector3_command::on_call()
	{
	}

	void vector3_command::save_state(nlohmann::json& value)
	{
		value = nlohmann::json::object();
		value["x"] = m_state.x;
		value["y"] = m_state.y;
		value["z"] = m_state.z;
	}

	void vector3_command::load_state(nlohmann::json& value)
	{
		if (value.is_object())
		{
			m_state.x = value["x"];
			m_state.y = value["y"];
			m_state.z = value["z"];
		}
	}

	vector3_command::vector3_command(std::string name, std::string label, std::string description, Vector3 def_val) :
	    command(name, label, description, 0),
	    m_state(def_val)
	{
	}

	Vector3 vector3_command::get_state()
	{
		return m_state;
	}

	void vector3_command::set_state(const Vector3& state)
	{
		g_fiber_pool->queue_job([this] { on_change(); });
		m_state = state;
		mark_dirty();
	}
}