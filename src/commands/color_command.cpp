#include "color_command.hpp"
#include "fiber_pool.hpp" // TODO: game import in core

namespace big
{
	void color_command::on_call()
	{
	}

	void color_command::save_state(nlohmann::json& value)
	{
		value = {m_color_state.x, m_color_state.y, m_color_state.z, m_color_state.w};
	}

	void color_command::load_state(nlohmann::json& value)
	{
		if (value.is_array() && value.size() == 4)
		{
			m_color_state = ImVec4(value[0], value[1], value[2], value[3]);
		}
		else
		{
			m_color_state = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	color_command::color_command(std::string name, std::string label, std::string description, ImVec4 color) :
	    command(name, label, description, 0),
		m_color_state(color)
	{
	}

	ImVec4 color_command::get_state()
	{
		return m_color_state;
	}

	void color_command::set_color_state(ImVec4 state)
	{
		g_fiber_pool->queue_job([this] { on_change(); });

		m_color_state = state;

		mark_dirty();
	}
}