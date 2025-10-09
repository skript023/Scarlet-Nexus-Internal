#include "bool_command.hpp"
#include "fiber_pool.hpp" // TODO: game import in core
#include "commands.hpp"

namespace big
{
	bool_command::bool_command(std::string name, std::string label, std::string description, bool def_value) :
	    command(name, label, description, 0),
	    m_state(def_value)
	{
		commands::add_bool_command(this);
	}

	void bool_command::on_call()
	{
		set_state(!m_state);
	}

	void bool_command::save_state(nlohmann::json& value)
	{
		value = m_state;
	}

	void bool_command::load_state(nlohmann::json& value)
	{
		m_state = value;
	}

	bool& bool_command::get_state()
	{
		return m_state;
	}

	void bool_command::set_state(bool state)
	{
		if (state && !m_state)
			on_enable();
		else if (!state && m_state)
			on_disable();

		m_state = state;

		mark_dirty();
	}

	void bool_command::initialize()
	{
		on_enable();
	}

	void bool_command::shutdown()
	{
		on_disable();
	}
}