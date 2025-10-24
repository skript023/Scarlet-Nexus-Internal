#include "state_serializer.hpp"
#include "settings.hpp"

namespace big
{
	state_serializer::state_serializer(const std::string& name) :
		m_ser_component_name(name),
		m_is_dirty(false)
	{
		settings::add_component(this);
	}
}