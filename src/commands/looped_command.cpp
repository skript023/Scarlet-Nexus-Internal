#include "looped_command.hpp"
#include "commands.hpp"

namespace big
{
	looped_command::looped_command(std::string name, std::string label, std::string description) : 
		bool_command(name, label, description)
	{
		commands::add_looped_command(this);
	}

	void looped_command::tick()
	{
		on_tick();
	}
}