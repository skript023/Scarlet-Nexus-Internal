#pragma once
#include "hardware.hpp"
#include "thread_pool.hpp"

namespace big
{
	class user : public hardware
	{
	public:
		explicit user()
		{
			g_thread_pool->push([this] {
				this->start_integration();
			});
		}

		virtual ~user() noexcept = default;

		user(const user& that) = delete;
		user& operator=(const user& that) = delete;

		user(user&& that) = delete;
		user& operator=(user&& that) = delete;

		bool start_integration()
		{
			nlohmann::json json = {
				{ "hardware", this->get_unique_id()},
			};

			try
			{
				cpr::Url route = std::format("{}{}", g_base_url, "/integration/grants-access");
				cpr::Body body = json.dump();
				cpr::Header header = {
					{"content-type", "application/json"}
				};

				auto response = cpr::Post(route, body, header);
				if (response.status_code != 200) return false;
				nlohmann::ordered_json j = nlohmann::ordered_json::parse(response.text.begin(), response.text.end());
				this->set_token(j["data"]["token"]);
				this->set_fullname(j["data"]["fullname"]);
				this->set_username(j["data"]["username"]);
				this->set_role(j["data"]["role"]);

				LOG(INFO) << j["message"];
			}
			catch (std::exception const&)
			{
				LOG(WARNING) << "failed integrate with server";

				return false;
			}

			return true;
		}

		void set_token(std::string token) { this->m_token = token; };
		void set_fullname(std::string fullname) { this->m_fullname = fullname; };
		void set_username(std::string username) { this->m_username = username; };
		void set_role(std::string role) { this->m_role = role; };

		NODISCARD std::string access_token() const { return m_token; };
		NODISCARD std::string fullname() const { return m_fullname; };
		NODISCARD std::string username() const { return m_username; };
		NODISCARD std::string role() const { return m_role; };
	private:
		std::string m_token;
		std::string m_fullname;
		std::string m_username;
		std::string m_role;
	};
}