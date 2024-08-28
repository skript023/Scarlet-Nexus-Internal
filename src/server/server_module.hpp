#pragma once
#include "alpha/alpha_service.hpp"
#include "alpha/alpha_module.hpp"

namespace big
{
    class server_module;
    inline server_module* g_server_module{};

    class server_module
    {
    public:
        explicit server_module()
        {
            alpha_module alpha;
            m_alpha_service = new alpha_service(alpha);
            m_alpha_service->ping();

            g_server_module = this;
        }

        ~server_module() noexcept
        {
            g_server_module->shutdown();
        }

        alpha_service* get_alpha() { return m_alpha_service; }

        void run()
        {
            m_alpha_service->poll();
        }
    private:
        void shutdown()
        {
            g_server_module = nullptr;
        }
    private:
        alpha_service* m_alpha_service;
    };
}