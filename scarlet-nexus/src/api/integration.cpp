#include "integration.hpp"
#include <script.hpp>

namespace big
{
	integration::integration() : m_user(std::make_unique<user>())
	{
		g_thread_pool->push([this] {
			this->signature();
		});
		g_web_server = this;
	}
	integration::~integration() noexcept
	{
		m_user.reset();

		g_web_server = nullptr;
	}
	bool integration::generate_injection_code()
	{
		try
		{
			nlohmann::json json = {
				{ "computer", this->get_computer_name() },
				{ "hardware", this->get_unique_id() }
			};

			cpr::Body body = json.dump(4);

			auto res = this->post("integration/generate-token");
			if (res.status_code != 200)
			{
				LOG(WARNING) << "Server not sending good response, connection terminated.";
				return false;
			}

			auto result = nlohmann::json::parse(res.text.begin(), res.text.end());
			g_running = result["injected"].get<bool>();
			//g_injection_code = json["injection_code"].get<uint32_t>();
		}
		catch (std::exception const&)
		{
			LOG(WARNING) << "Failed request to server.";
			return false;
		}

		return true;
	}
	bool integration::heartbeat()
	{
		try
		{
			auto res = this->get(std::format("integration/heartbeat/device-check?computer={}&hardware={}", this->get_computer_name(), this->get_unique_id()));
			if (res.status_code != 200) return false;

			auto json = nlohmann::json::parse(res.text.begin(), res.text.end());

			LOG(INFO) << json["message"];
		}
		catch (std::exception const&)
		{
			LOG(WARNING) << "Failed connect to server";
			return false;
		}

		return true;
	}
	bool integration::signature()
	{
		try
		{
			auto res = this->get(std::format("integration/signature?game={}", "Scarlet-Nexus"));
			if (res.status_code != 200) return false;

			auto json = nlohmann::json::parse(res.text.begin(), res.text.end());
			this->set_signature(json["signatures"]);

			LOG(INFO) << json["message"];
		}
		catch (std::exception const&)
		{
			LOG(WARNING) << "Failed connect to server";
			return false;
		}

		return true;
	}
}