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
        virtual ~IServerConnection() = 0;

        /*
         * Send a packet to the server.
         */
        virtual void SendPacket(Packet& a_Data, size_t a_Size) = 0;

        /*
         * Send a packet to the server with type awareness.
         */
        template<typename T>
        void SendPacket(T& a_Data)
        {
            static_assert(std::is_base_of_v<Packet, T>, "Can only send templated packet with a class derived from Packet.");
            SendPacket(&a_Data, sizeof(T));
        }

        /*
         * Called when a packet is received from the server.
         */
        virtual void ReceivePacket(PacketType a_Type, void* a_Data) = 0;

        /*
         * Called to process all packets that have been queued up.
         */
        virtual void ProcessPackets() = 0;

        /*
         * Try to connect to the given IP address.
         */
        virtual void Connect(const std::string& a_Ip) = 0;

        /*
         * Disconnect if connected to a server.
         */
        virtual void Disconnect() = 0;

        /*
         * Get the current state of the connection.
         */
        virtual ConnectionState GetConnectionState() = 0;

        /*
         * Get the packet manager used to handle incoming packets.
         */
        virtual IPacketManager& GetPacketManager() = 0;

        /*
         * Get the last timestamp at which a response from the server was received.
         */
        virtual std::uint64_t GetLastResponse() = 0;

        /*
         * Get the timestamp when the connection was first made.
         */
        virtual std::uint64_t GetConnectionStartTime() = 0;
    };
}
