#pragma once
#include "alpha/alpha_service.hpp"

namespace big
{
    class server_module;
    inline server_module* g_server_module{};

    class server_module
    {
    public:
        explicit server_module()
        {
            m_alpha_service = std::make_shared<alpha_service>();

            g_server_module = this;
        }

        ~server_module() noexcept
        {
            g_server_module->shutdown();
        }

        alpha_service* get_alpha() { return m_alpha_service.get(); }

        void run()
        {
            if (!m_alpha_service->auto_reconnect())
            {
                m_alpha_service->ping();
                m_alpha_service->send_hardware();
            }
        }
    private:
        void shutdown()
        {
            m_alpha_service.reset();

            g_server_module = nullptr;
        }
    private:
        std::shared_ptr<alpha_service> m_alpha_service;
    };
}