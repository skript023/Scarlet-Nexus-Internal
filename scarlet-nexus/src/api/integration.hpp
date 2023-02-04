#pragma once
#include "users.hpp"

namespace big
{
	class integration : public user
	{
	public:
		explicit integration();
		~integration() noexcept;

		integration(const integration& that) = delete;
		integration& operator=(const integration& that) = delete;

		integration(integration&& that) = delete;
		integration& operator=(integration&& that) = delete;

		bool generate_injection_code();
		bool request_globals(nlohmann::ordered_json& global_json);
		bool request_locals(nlohmann::ordered_json& local_json);
		virtual void heartbeat();
		static void real_time_integration();
		static void real_time_request();

		template<class ...Args>
		static cpr::Response get(std::string route, Args&& ...args)
		{
			cpr::Url url = std::format("{}/{}", g_base_url, route);
			return cpr::Get(std::forward<Args>(args)..., url, cpr::Header{ {"Authorization", this->access_token()} });
		}

		template<class ...Args>
		static cpr::Response post(std::string route, Args&& ...args)
		{
			cpr::Url url = std::format("{}/{}", g_base_url, route);
			return cpr::Post(std::forward<Args>(args)..., url, cpr::Header{ {"Authorization", this->access_token()} });
		}

		user* users() { return m_user.get(); }
	private:
		std::unique_ptr<user> m_user;
	};

	inline std::unique_ptr<integration> g_web_server{};
}