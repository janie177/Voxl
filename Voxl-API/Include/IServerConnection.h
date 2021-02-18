#pragma once
#include <string>

#include "ConnectionState.h"
#include "IConnection.h"
#include "PacketType.h"

namespace voxl
{
    struct IPacket;
    class IPacketManager;

    /*
     * An interface to handle a connection from client to server.
     * Offers functionality to listen to packets and process them.
     */
    class IServerConnection : public IConnection
    {
    public:
        /*
         * Try to connect to the given IP address.
         * If connections succeeds, an authentication packet is sent.
         */
        virtual bool Connect(const std::string& a_Ip, std::uint32_t a_Port, const Packet_Authenticate& a_Authentication) = 0;

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
