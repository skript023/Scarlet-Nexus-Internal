#pragma once

#include "gateway.hpp"

namespace big
{
    template<class T>
    struct Message : public Gateway
    {
        T data;

        nlohmann::json to_json()
        {
            return *this;
        }

        void operator=(nlohmann::json const& data)
        {
            *this = data.get<Request>();
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Request, event, data)
    };
}