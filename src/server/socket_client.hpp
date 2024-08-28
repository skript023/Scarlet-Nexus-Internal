#pragma once
#include "easywsclient.hpp" // Include the easywsclient header

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

namespace big
{
	class socket_client
	{
	public:
		using MessageCallback = std::function<void(const std::string&)>;

		socket_client(const std::string& host, int port)
		{
			// Initialize Winsock
			WSADATA wsaData;
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			{
				throw std::runtime_error("WSAStartup failed");
			}

			std::string url = "ws://" + host + ":" + std::to_string(port);

			LOG(HACKER) << "Connecting to " << url;

			ws_             = easywsclient::WebSocket::from_url(url);

			if (!ws_)
			{
				WSACleanup();
				throw std::runtime_error("Error: Connection failed");
			}

			LOG(HACKER) << "Connected to " << url;
		}

		~socket_client()
		{
			if (ws_)
			{
				ws_->close();
				delete ws_;
			}
			WSACleanup(); // Cleanup Winsock
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
				message_callback_.push(std::move(callback));
			}
		}

		void poll()
		{
			if (ws_)
			{
				ws_->poll();
				ws_->dispatch([this](const std::string& message) {
					std::unique_lock lock(m_mutex);
					if (!message_callback_.empty())
					{
						auto job = std::move(message_callback_.top());
						message_callback_.pop();
						lock.unlock();

						std::invoke(std::move(job), message);
					}
				});
			}
		}

		bool is_connected() const
		{
			return ws_ && ws_->getReadyState() != easywsclient::WebSocket::CLOSED;
		}

	private:
		easywsclient::WebSocket* ws_ = nullptr;
		std::stack<MessageCallback> message_callback_;
		std::recursive_mutex m_mutex;
	};
}
