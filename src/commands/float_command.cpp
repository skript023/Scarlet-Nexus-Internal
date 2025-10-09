#include "float_command.hpp"
#include "fiber_pool.hpp" // TODO: game import in core

namespace big
{
	void float_command::on_call()
	{
	}

	void float_command::save_state(nlohmann::json& value)
	{
		value = m_state;
	}

	void float_command::load_state(nlohmann::json& value)
	{
		m_state = value;
	}

	float_command::float_command(std::string name, std::string label, std::string description, float min, float max, float def_val) :
	    command(name, label, description, 0),
	    m_min(min),
	    m_max(max),
	    m_state(def_val)
	{
	}

	float& float_command::get_state()
	{
		return m_state;
	}

	void float_command::set_state(float state)
	{
		g_fiber_pool->queue_job([this] { on_change(); });
		m_state = state;
		mark_dirty();
	}

	float float_command::get_minimum()
	{
		return m_min;
	}

	float float_command::get_maximum()
	{
		return m_max;
	}
}