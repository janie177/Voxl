#pragma once

namespace voxl
{
    enum class ConnectionState
    {
        CONNECTED,      //Connection is active.
        DISCONNECTED,   //No connection is happening.
        CONNECTING      //Trying to connect without result so far.
    };
}