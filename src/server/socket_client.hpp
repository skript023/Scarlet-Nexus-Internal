#pragma once
#include "easywsclient.hpp" // Include the easywsclient header

#include <winsock2.h>
#include <ws2tcpip.h>

#include "enums/connection_status.hpp"

#pragma comment(lib, "Ws2_32.lib")

namespace big
{
	class socket_client
	{
	public:
		using MessageCallback = std::function<void(const std::string&)>;

		socket_client(const std::string& host, int port):
			host_(host), port_(port), ws_(nullptr), m_connection_status(eConnectionStatus::DISCONNECT)
		{
			if (!connect())
			{
				LOG(WARNING) << "Error: Connection failed";
			}
		}

		~socket_client()
		{
			if (ws_)
			{
				ws_->close();
				delete ws_;
			}
			WSACleanup();

			m_connection_status = eConnectionStatus::DISCONNECT;
		}

		bool connect()
		{
			WSADATA wsaData;
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			{
				LOG(WARNING) << "WSAStartup failed";

				return false;
			}

			std::string url = std::format("ws://{}:{}", host_, port_);;

			LOG(HACKER) << "Connecting to " << url;

			m_connection_status = eConnectionStatus::CONNECTING;

			ws_                 = easywsclient::WebSocket::from_url(url);

			if (!ws_)
			{
				WSACleanup();
				
				LOG(WARNING) << "Error: Connection failed";

				return false;
			}

			LOG(HACKER) << "Connected to " << url;

			m_connection_status = eConnectionStatus::CONNECTED;

			return true;
		}

		void disconnect()
		{
			if (ws_)
			{
				ws_->close();
				delete ws_;
			}
			WSACleanup();
		}

		void send_message(const std::string& message)
		{
			if (ws_)
			{
				ws_->send(message);
			}
		}

		void on_message_received(MessageCallback callback)
		{
			if (callback)
			{
				std::lock_guard lock(m_mutex);
				m_message_callback.push(std::move(callback));
			}
		}

		void poll()
		{
			if (this->is_connected())
			{
				ws_->poll();
				ws_->dispatch([this](const std::string& message) {
					std::unique_lock lock(m_mutex);
					if (!m_message_callback.empty())
					{
						auto job = std::move(m_message_callback.top());
						m_message_callback.pop();
						lock.unlock();

						std::invoke(std::move(job), message);
					}
				});
			}
			else
			{
				m_connection_status = eConnectionStatus::DISCONNECT;
			}
		}

		bool is_connected() const
		{
			return ws_ && ws_->getReadyState() != easywsclient::WebSocket::CLOSED;
		}

		eConnectionStatus get_connection_status() const { return m_connection_status; }

	private:
		std::string host_;
		int port_;
		easywsclient::WebSocket* ws_ = nullptr;
		std::stack<MessageCallback> m_message_callback;
		std::recursive_mutex m_mutex;
		eConnectionStatus m_connection_status;
	};
}
