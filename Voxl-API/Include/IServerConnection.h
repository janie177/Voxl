#pragma once
#include <string>
#include <functional>

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

        /*
         * Wait for a specific packet.
         * This will discard other packets.
         * If no packets have been received before the timeout time is reached, false is returned.
         * If a packet was received, true is returned and the registered callback will be executed.
         */
        virtual bool WaitForPacket(PacketType a_Type, std::uint32_t a_TimeOutMillis, std::function<void(IPacket* a_Packet)> a_OnReceive) = 0;
    };
}
