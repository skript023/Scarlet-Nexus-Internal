#include "settings.hpp"

#include "state_serializer.hpp"
#include "settings.hpp"


namespace big
{
	settings::settings() :
		m_settings_file(),
		m_state_serializers(),
		m_initial_load_done(false)
	{
	}

	void settings::initialize_impl(file settingsFile)
	{
		m_settings_file = settingsFile.get_path();

		if (!settingsFile.exists())
		{
			reset();
			return;
		}

		std::ifstream file(m_settings_file);

		try
		{
			file >> m_json;
			file.close();
		}
		catch (std::exception&)
		{
			LOG(WARNING) << "Detected corrupt settings, resetting settings...";
			reset();
			return;
		}

		for (auto& serializer : m_state_serializers)
			load_component_impl(serializer);

		LOG(VERBOSE) << "All settings loaded";
		m_initial_load_done = true;
	}

	void settings::tick_impl()
	{
		std::lock_guard lock(m_mutex);
		while (!m_late_loaders.empty())
		{
			if (auto component = std::move(m_late_loaders.front()))
			{
				load_component(component);
			}

			m_late_loaders.pop();
		}

		if (m_initial_load_done && should_save())
		{
			for (auto& serializer : m_state_serializers)
				if (serializer->is_state_dirty())
					save_component_impl(serializer);

			std::ofstream file(m_settings_file, std::ios::out | std::ios::trunc);
			file << m_json.dump(4);
			file.close();
		}
	}

	void settings::add_component_impl(state_serializer* serializer)
	{
		std::lock_guard lock(m_mutex);
		m_state_serializers.push_back(serializer);
		if (m_initial_load_done)
			m_late_loaders.push(serializer);
	}

	void settings::load_component_impl(state_serializer* serializer)
	{
		LOG(VERBOSE) << "Loading component: " << serializer->get_serializer_component_name();

		if (!m_json.contains(serializer->get_serializer_component_name()) || !m_json[serializer->get_serializer_component_name()].is_object())
			m_json[serializer->get_serializer_component_name()] = nlohmann::json::object();

		serializer->load_state(m_json[serializer->get_serializer_component_name()]);
	}

	void settings::save_component_impl(state_serializer* serializer)
	{
		//LOG(VERBOSE) << "Saving component: " << serializer->get_serializer_component_name();
		serializer->save_state(m_json[serializer->get_serializer_component_name()]);
	}

	void settings::reset()
	{
		std::ofstream file(m_settings_file, std::ios::out | std::ios::trunc);
		file << "{}" << std::endl;
		file.close();
		m_json = "{}";
	}

	bool settings::should_save()
	{
		for (auto& serializer : m_state_serializers)
			if (serializer->is_state_dirty())
				return true;

		return false;
	}
}