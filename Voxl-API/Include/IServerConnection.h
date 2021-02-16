#pragma once
#include <string>

#include "ConnectionState.h"
#include "PacketType.h"

namespace voxl
{
    struct Packet;
    class IPacketManager;

    /*
     * An interface to handle a connection from client to server.
     * Offers functionality to listen to packets and process them.
     */
    class IServerConnection
    {
    public:
        /*
         * Try to connect to the given IP address.
         */
        virtual void Connect(const std::string& a_Ip) = 0;

        /*
         * Get the packet manager used to handle incoming packets.
         */
        virtual IPacketManager& GetPacketManager() = 0;

        /*
         * Called to process all packets that have been queued up.
         */
        virtual void ProcessPackets() = 0;
    };
}
