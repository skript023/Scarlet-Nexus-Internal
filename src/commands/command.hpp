#pragma once
#include "utility/joaat.hpp"

#include <nlohmann/json.hpp>


namespace big
{
	class command
	{
	private:
		std::string m_name;
		std::string m_label;
		std::string m_description;
		joaat_t m_hash;

		int m_num_args = 0; // TODO: currently unused

	protected:
		virtual void on_call() = 0;
		void mark_dirty();

	public:
		command(std::string name, std::string label, std::string description, int num_args = 0);
		void call();

		virtual void save_state(nlohmann::json& value){};
		virtual void load_state(nlohmann::json& value){};

		const std::string& get_name()
		{
			return m_name;
		}

		const std::string& get_label()
		{
			return m_label;
		}

		const std::string& get_description()
		{
			return m_description;
		}

		joaat_t get_hash()
		{
			return m_hash;
		}
	};
}