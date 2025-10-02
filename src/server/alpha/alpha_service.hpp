#pragma once
#include "data/hardware.hpp"
#include "server/socket_client.hpp"
#include "server/interfaces/message.hpp"

#include "notification/notification_service.hpp"

namespace big
{
	class alpha_service
	{
	public:
		explicit alpha_service()
		{
			m_alpha_gateway = std::make_shared<socket_client>("quantum.rena.my.id/ws/auth");
			m_alpha_gateway->on_message_received([](const std::string& message) {
				if (!message.empty())
				{
					notification::info("Server", message);
				}
			});
		}

		~alpha_service() noexcept
		{
			m_alpha_gateway->disconnect();

			m_alpha_gateway.reset();
		}

		void find()
		{
			m_event.event = "findAllAlpha";
			
			m_alpha_gateway->send_message(m_event.to_json_string());
			m_alpha_gateway->on_message_received([](const std::string& message) {
				if (message.empty())
				{
					LOG(INFO) << "Received an empty message";
				}
				else
				{
					LOG(INFO) << "Received: " << message;
				}
			});
		}

		void ping()
		{
			m_event.event = "pingAlpha";
			
			m_alpha_gateway->send_message(m_event.to_json_string());
			m_alpha_gateway->on_message_received([](const std::string& message) {
				if (message.empty())
				{
					LOG(INFO) << "Received an empty message";
				}
				else
				{
					LOG(INFO) << "Received: " << message;
				}
			});
		}

		void send_hardware()
		{

		}

		bool auto_reconnect()
		{
			if (!m_alpha_gateway->is_connected())
			{
				return m_alpha_gateway->reconnect();
			}

			return false;
        }
	private:
		std::shared_ptr<socket_client> m_alpha_gateway;
		Gateway m_event;
	};
}
