#pragma once

namespace big
{
    struct Gateway
    {
        std::string event;
        std::string payload;

        nlohmann::json to_json()
        {
            return *this;
        }

        std::string to_json_string()
        {
            return nlohmann::json(*this).dump();
        }

        void operator=(nlohmann::json const& data)
        {
            *this = data.get<Gateway>();
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Gateway, event, payload)
    };
}