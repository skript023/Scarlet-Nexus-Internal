#include "list_command.hpp" 
#include "fiber_pool.hpp" // TODO: game import in core

namespace big
{
	void list_command::on_call()
	{
	}

	void list_command::save_state(nlohmann::json& value)
	{
		value = m_state;
	}

	void list_command::load_state(nlohmann::json& value)
	{
		m_state = value;
	}

	list_command::list_command(std::string name, std::string label, std::string description, std::vector<std::pair<int, const char*>> list, int def_val) :
	    command(name, label, description, 0),
	    m_list(list),
	    m_state(def_val)
	{
	}

	int list_command::get_state()
	{
		return m_state;
	}

	void list_command::set_state(int state)
	{
		g_fiber_pool->queue_job([this] { on_change(); });
		m_state = state;
		mark_dirty();
	}

	std::vector<std::pair<int, const char*>>& list_command::get_list()
	{
		return m_list;
	}
}