#pragma once
#include "utility/joaat.hpp"
#include "settings/state_serializer.hpp"

namespace big
{
	class command;
	class looped_command;
	class bool_command;
	class process_event;

	class commands : private state_serializer
	{
	private:
		std::unordered_map<joaat_t, command*> m_commands;
		std::vector<looped_command*> m_looped_commands;
		std::vector<bool_command*> m_bool_commands;
		std::unordered_map<joaat_t, process_event*> m_process_event_commands;

		commands();

	public:
		static void add_command(command* command)
		{
			get_instance().add_command_impl(command);
		}

		static void add_bool_command(bool_command* command)
		{
			get_instance().add_bool_command_impl(command);
		}

		static void add_looped_command(looped_command* command)
		{
			get_instance().add_looped_command_impl(command);
		}
		
		/*static void add_process_event_command(process_event* command)
		{
			get_instance().add_process_event_command_impl(command);
		}*/

		static void run_looped_command()
		{
			get_instance().run_looped_commands_impl();
		}

		static void enable_bool_commands()
		{
			get_instance().enable_bool_commands_impl();
		}

		template<typename T = command>
		static T* get_command(joaat_t hash)
		{
			return reinterpret_cast<T*>(get_instance().get_command_impl(hash));
		}

		static process_event* get_process_event(joaat_t hash)
		{
			return get_instance().get_process_event_impl(hash);
		}

		static std::unordered_map<joaat_t, command*> get_command()
		{
			return get_instance().m_commands;
		};
		
		static std::vector<looped_command*> get_looped_commands()
		{
			return get_instance().m_looped_commands;
		}

		static void mark_dirty()
		{
			get_instance().mark_state_dirty();
		}
		
		static void shutdown()
		{
			get_instance().shutdown_impl();
		}

	private:
		void add_command_impl(command* command);
		void add_bool_command_impl(bool_command* command);
		void add_looped_command_impl(looped_command* command);
		//void add_process_event_command_impl(process_event* command);
		void enable_bool_commands_impl();
		void run_looped_commands_impl();
		command* get_command_impl(joaat_t hash);
		process_event* get_process_event_impl(joaat_t hash);
		virtual void save_state_impl(nlohmann::json& state) override;
		virtual void load_state_impl(nlohmann::json& state) override;
		void shutdown_impl();

		static commands& get_instance()
		{
			static commands instance{};
			return instance;
		}
	};
}