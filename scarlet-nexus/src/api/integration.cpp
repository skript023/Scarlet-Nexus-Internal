#include "integration.hpp"
#include <script.hpp>

namespace big
{
	integration::integration() : m_user(std::make_unique<user>())
	{

	}
	integration::~integration() noexcept
	{
		m_user.reset();
	}
	bool integration::generate_injection_code()
	{
		try
		{
			auto res = integration::get(std::format("injection/begin/{}", this->get_bios()));
			if (res.status_code != 200)
			{
				LOG(WARNING) << "Server not sending good response, connection terminated.";
				return false;
			}

			auto json = nlohmann::json::parse(res.text.begin(), res.text.end());
			g_running = json["injected"].get<bool>();
			//g_injection_code = json["injection_code"].get<uint32_t>();
		}
		catch (std::exception const&)
		{

			return false;
		}

		return true;
	}
	bool integration::request_globals(nlohmann::ordered_json& global_json)
	{
		try
		{
			auto res = integration::get("variable/global");
			if (res.status_code != 200)
			{
				LOG(WARNING) << "Server not sending good response, connection terminated.";
				return false;
			}

			auto json = nlohmann::ordered_json::parse(res.text.begin(), res.text.end());

			global_json = json;
		}
		catch (std::exception const&)
		{
			LOG(WARNING) << "Failed request data from server";
			return false;
		}

		return true;
	}
	bool integration::request_locals(nlohmann::ordered_json& local_json)
	{
		try
		{
			auto res = integration::get("variable/local");
			if (res.status_code != 200)
			{
				LOG(WARNING) << "Server not sending good response, connection terminated.";
				return false;
			}

			auto json = nlohmann::ordered_json::parse(res.text.begin(), res.text.end());

			local_json = json;
		}
		catch (std::exception const&)
		{
			LOG(WARNING) << "Failed request data from server";
			return false;
		}

		return true;
	}
	void integration::heartbeat()
	{
		try
		{
			auto res = integration::get("heartbeat/device-check");
			if (res.status_code != 200) return;

			auto json = nlohmann::json::parse(res.text.begin(), res.text.end());

			LOG(INFO) << json["message"];
		}
		catch (std::exception const&)
		{
			LOG(WARNING) << "Failed connect to server";
		}
	}
	void integration::real_time_integration()
	{
		g_web_server->heartbeat();
	}
	void integration::real_time_request()
	{
		TRY_CLAUSE
		{
			while (g_running)
			{
				static auto start = std::chrono::high_resolution_clock::now();

				if ((start - std::chrono::high_resolution_clock::now()).count() < std::chrono::minutes(1).count())
				{
					real_time_integration();
				}
			}
		}
			EXCEPT_CLAUSE
	}
}