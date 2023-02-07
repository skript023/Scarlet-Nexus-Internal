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

		virtual bool generate_injection_code();
		virtual bool heartbeat();
		virtual bool signature();

		template<class ...Args>
		cpr::Response get(std::string_view route, Args&& ...args)
		{
			cpr::Url url = std::format("{}/{}", g_base_url, route);
			return cpr::Get(std::forward<Args>(args)..., url, cpr::Header{ {"Authorization", this->access_token()} });
		}

		template<class ...Args>
		cpr::Response post(std::string_view route, Args&& ...args)
		{
			cpr::Url url = std::format("{}/{}", g_base_url, route);
			return cpr::Post(std::forward<Args>(args)..., url, cpr::Header{ {"Authorization", this->access_token()} });
		}

		template<class ...Args>
		cpr::Response update(std::string_view route, Args&& ...args)
		{
			cpr::Url url = std::format("{}/{}", g_base_url, route);
			return cpr::Put(std::forward<Args>(args)..., url, cpr::Header{ {"Authorization", this->access_token()} });
		}

		template<class ...Args>
		cpr::Response remove(std::string_view route, Args&& ...args)
		{
			cpr::Url url = std::format("{}/{}", g_base_url, route);
			return cpr::Delete(std::forward<Args>(args)..., url, cpr::Header{ {"Authorization", this->access_token()} });
		}

		void set_signature(nlohmann::json json) { m_signature = json; }
		NODISCARD std::string_view get_signature(std::string signature_name) { return m_signature["signatures"][signature_name]; }

		user* users() { return m_user.get(); }
	private:
		nlohmann::json m_signature{};
		std::unique_ptr<user> m_user;
	};

	inline integration* g_web_server{};
}