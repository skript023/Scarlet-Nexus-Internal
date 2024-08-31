#pragma once

namespace big
{
    enum class eConnectionStatus : uint32_t
    {
        CONNECTING = 1 << 8,
        CONNECTED = 2 << 16,
        DISCONNECT = 3 << 24
    };
}