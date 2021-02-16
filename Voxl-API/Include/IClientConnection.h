#pragma once
#include <string>


#include "IConnection.h"
#include "ConnectionState.h"
#include "PacketType.h"

namespace voxl
{
    class IClientConnection : public IConnection
    {
    public:
        /*
         * Get the username of this connection.
         */
        virtual std::string GetUsername() const = 0;
    };
}
