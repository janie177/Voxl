#pragma once

namespace voxl
{
    enum class ConnectionState
    {
        CONNECTED,      //Connection is active.
        CONNECTING,     //Attempting to connect.
        DISCONNECTED    //No connection is happening.
    };
}