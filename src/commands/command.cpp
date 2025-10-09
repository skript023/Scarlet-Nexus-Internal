#include "utility/joaat.hpp"
#include "command.hpp"
#include "commands.hpp"

namespace big
{
	command::command(std::string name, std::string label, std::string description, int num_args) :
	    m_name(name),
	    m_label(label),
	    m_description(description),
	    m_num_args(num_args),
	    m_hash(joaat(name))
	{
		commands::add_command(this);
	}

	void command::call()
	{
		on_call();
	}

	void command::mark_dirty()
	{
		commands::mark_dirty();
	}
}