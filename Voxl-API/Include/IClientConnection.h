#pragma once
#include <string>

#include "ConnectionState.h"
#include "PacketType.h"

namespace voxl
{
    class IPacketManager;

    class IClientConnection
    {
    public:
        virtual ~IClientConnection() = 0;

        /*
        * Send a packet to the client.
        */
        virtual void SendPacket(Packet& a_Data, size_t a_Size) = 0;

        /*
         * Send a packet to the client with type awareness.
         */
        template<typename T>
        void SendPacket(T& a_Data)
        {
            static_assert(std::is_base_of_v<Packet, T>, "Can only send templated packet with a class derived from Packet.");
            SendPacket(&a_Data, sizeof(T));
        }

        /*
         * Called when a packet is received from the client.
         */
        virtual void ReceivePacket(PacketType a_Type, void* a_Data) = 0;

        /*
         * Called to process all packets that have been queued up.
         */
        virtual void ProcessPackets() = 0;

        /*
         * Disconnect the client.
         */
        virtual void Disconnect() = 0;

        /*
         * Get the current state of the connection to the client.
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
