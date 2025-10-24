#pragma once
#include "file_manager/file.hpp"

namespace big
{
	class state_serializer;

	class settings
	{
	private:
		std::filesystem::path m_settings_file;
		std::vector<state_serializer*> m_state_serializers;
		std::queue<state_serializer*> m_late_loaders;
		bool m_initial_load_done;
		nlohmann::json m_json;
		std::mutex m_mutex;

	public:
		settings();

		static void initialize(file settingsFile)
		{
			get_instance().initialize_impl(settingsFile);
		}

		static void tick()
		{
			get_instance().tick_impl();
		}

		static void add_component(state_serializer* serializer)
		{
			get_instance().add_component_impl(serializer);
		}

		// TODO: this is broken
		static void load_component(state_serializer* serializer)
		{
			get_instance().load_component_impl(serializer);
		}

		static bool initial_load_done()
		{
			return get_instance().m_initial_load_done;
		}

		static std::string get_file_path()
		{
			return get_instance().m_settings_file.string();
		}

	private:
		static settings& get_instance()
		{
			static settings Instance;
			return Instance;
		}

		void initialize_impl(file settingsFile);
		void tick_impl();
		void add_component_impl(state_serializer* serializer);
		void load_component_impl(state_serializer* serializer);
		void save_component_impl(state_serializer* serializer);
		void reset();
		bool should_save();
	};
}