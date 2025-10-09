#pragma once
#include "bool_command.hpp"

namespace big
{
	class looped_command : public bool_command
	{
	protected:
		virtual void on_tick() = 0;

	public:
		looped_command(std::string name, std::string label, std::string description);
		void tick();
	};
}