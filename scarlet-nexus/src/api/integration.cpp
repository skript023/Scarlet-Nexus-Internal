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
	bool integration::begin_integration()
	{
		try
		{
			auto res = integration::get(std::format("injection/begin/{}", 1));
		}
		catch (std::exception const&)
		{

			return false;
		}

		return true;
	}
	bool integration::request_globals(nlohmann::ordered_json &global_json)
	{
		try
		{
			auto res = integration::get("variable/global");
			if (res.status_code != 200)
			{
				LOG(WARNING) << "Server not giving good response, rejected.";
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
	void integration::heartbeat()
	{
		try
		{
			integration::post("");

		}
		catch (std::exception const&)
		{

		}
	}
	void integration::real_time_integration()
	{

	}
	void integration::real_time_request()
	{
		TRY_CLAUSE
		{
			while (g_running)
			{
				static auto start = std::chrono::high_resolution_clock::now();

				if ((start - std::chrono::high_resolution_clock::now()).count() < std::chrono::seconds(10).count())
				{
					real_time_integration();
				}
			}
		}
		EXCEPT_CLAUSE
	}
}