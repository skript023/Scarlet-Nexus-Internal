#pragma once

namespace big
{
	class state_serializer
	{
		std::string m_ser_component_name;
		bool m_is_dirty;
	public:
		state_serializer(const std::string& name);
		virtual void save_state_impl(nlohmann::json& state) = 0;
		virtual void load_state_impl(nlohmann::json& state) = 0;

		inline void save_state(nlohmann::json& state)
		{
			save_state_impl(state);
			m_is_dirty = false;
		}

		inline void load_state(nlohmann::json& state)
		{
			load_state_impl(state);
			m_is_dirty = false;
		}

		inline bool is_state_dirty()
		{
			return m_is_dirty;
		}

		inline void mark_state_dirty()
		{
			m_is_dirty = true;
		}

		inline const std::string& get_serializer_component_name()
		{
			return m_ser_component_name;
		}
	};
}