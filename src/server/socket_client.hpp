#pragma once

#define ASIO_STANDALONE

#include "client_wss.hpp"
#include "thread_pool.hpp"
#include "enums/connection_status.hpp"

namespace big
{
    using WsClient = SimpleWeb::SocketClient<SimpleWeb::WSS>;

    class socket_client
    {
    public:
        using MessageCallback = std::function<void(std::string const&)>;

        socket_client(std::string const& host)
            : host_(host), m_connection_status(eConnectionStatus::DISCONNECT)
        {
            connect();  // Try to connect on initialization
        }

        ~socket_client()
        {
            disconnect();  // Disconnect on destruction
        }

        // Attempt to connect or reconnect to the WebSocket server
        bool connect()
        {
            if (m_connection_status == eConnectionStatus::CONNECTED)
            {
                LOG(INFO) << "Already connected.";
                return true;
            }

            disconnect();  // Ensure any previous connection is stopped before reconnecting
            std::string url = host_;

            LOG(INFO) << "Connecting to " << url;

            client = std::make_shared<WsClient>(url, false);  // Create WebSocket client instance

            // Define what happens when a connection is established
            client->on_open = [this](std::shared_ptr<WsClient::Connection> connection) {
                m_connection_status = eConnectionStatus::CONNECTED;
                active_connection_ = connection;  // Store the active connection

                LOG(INFO) << "Connected to " << host_;
            };

            // Define what happens when a message is received
            client->on_message = [this](std::shared_ptr<WsClient::Connection> connection, std::shared_ptr<WsClient::InMessage> message) {
                std::unique_lock lock(m_mutex);
                if (!m_message_callback.empty())
                {
                    auto job = std::move(m_message_callback.top());
                    m_message_callback.pop();
                    lock.unlock();
                    job(message->string());
                }
            };

            // Define what happens when the connection is closed
            client->on_close = [this](std::shared_ptr<WsClient::Connection> connection, int status, const std::string& reason) {
                LOG(WARNING) << "Connection closed, Status: " << status << " Reason: " << reason;
                m_connection_status = eConnectionStatus::DISCONNECT;
            };

            // Define what happens when there is an error
            client->on_error = [this](std::shared_ptr<WsClient::Connection> connection, const SimpleWeb::error_code& ec) {
                LOG(FATAL) << "Error: [" << ec.value() << "] Reason: " << ec.message();
                m_connection_status = eConnectionStatus::DISCONNECT;
            };

            // Start the WebSocket client in a separate thread
            g_thread_pool->push([this]
            {
                LOG(INFO) << "Starting WebSocket client...";
                client->start();
                LOG(INFO) << "WebSocket client has been started.";
            });

            return true;
        }

        // Gracefully stop and disconnect the WebSocket client
        void disconnect()
        {
            if (client)
            {
                LOG(INFO) << "Stopping WebSocket client...";
                client->stop();  // Stop the client
                active_connection_.reset();  // Clear the active connection
                m_connection_status = eConnectionStatus::DISCONNECT;
                LOG(INFO) << "WebSocket client stopped.";
            }
        }

        // Reconnect logic
        bool reconnect()
        {
            LOG(INFO) << "Reconnecting...";
            disconnect();  // First, disconnect the previous session
            return connect();  // Then, attempt to reconnect
        }

        void send_message(const std::string& message)
        {
            if (client && m_connection_status == eConnectionStatus::CONNECTED && active_connection_)
            {
                active_connection_->send(message);  // Send the message through the active connection
            }
            else
            {
                LOG(WARNING) << "No active connection to send the message.";
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

        bool is_connected() const
        {
            return m_connection_status == eConnectionStatus::CONNECTED;
        }

        eConnectionStatus get_connection_status() const
        {
            return m_connection_status;
        }

    private:
        std::string host_;
        std::shared_ptr<WsClient> client;  // WebSocket client instance
        std::thread client_thread;
        std::stack<MessageCallback> m_message_callback;
        std::recursive_mutex m_mutex;
        eConnectionStatus m_connection_status;
        std::shared_ptr<WsClient::Connection> active_connection_;  // Store the active connection
    };
}
