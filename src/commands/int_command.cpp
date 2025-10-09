#include "int_command.hpp"
#include "fiber_pool.hpp" // TODO: game import in core

namespace big
{
	void int_command::on_call()
	{
	}

	void int_command::save_state(nlohmann::json& value)
	{
		value = m_state;
	}

	void int_command::load_state(nlohmann::json& value)
	{
		m_state = value;
	}

	int_command::int_command(std::string name, std::string label, std::string description, std::optional<int> min, std::optional<int> max, int def_val) :
	    command(name, label, description, 0),
	    m_min(min),
	    m_max(max),
	    m_state(def_val)
	{
	}

	int int_command::get_state()
	{
		return m_state;
	}

	void int_command::set_state(int state)
	{
		g_fiber_pool->queue_job([this] { on_change(); });
		m_state = state;
		mark_dirty();
	}

	std::optional<int> int_command::get_minimum()
	{
		return m_min;
	}

	std::optional<int> int_command::get_maximum()
	{
		return m_max;
	}
}