#pragma once
#include <iostream>
#include <string>

#include "IPacketHandler.h"
#include "PacketType.h"

namespace voxl
{
    class ConnectionManager;

    class PacketHandler_IncomingMessage : public PacketHandler<Packet_ChatMessage>
    {
    public:
        PacketHandler_IncomingMessage()
        {

        }

        bool OnResolve(Packet_ChatMessage& a_Data, IConnection* a_Sender) override
        {
            //Copy the packet and fill in the username. Then send it to everyone.

            std::cout << "[" << a_Data.sender << "] " << a_Data.message << std::endl;

            return true;
        }
    };
}
