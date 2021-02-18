#pragma once
#include <cinttypes>
#include <string>


#include "ConnectionState.h"

namespace voxl
{
    enum class PacketType;
    struct IPacket;

    /*
     * Interface for retrieving information about a connection.
     */
    class IConnection
    {
    public:
        virtual ~IConnection() = default;

        /*
         * Disconnect if connected.
         */
        virtual void Disconnect() = 0;

        /*
         * Get the current state of the connection.
         */
        virtual ConnectionState GetConnectionState() = 0;

        /*
         * Get the last timestamp at which a response from the server was received.
         */
        virtual std::uint64_t GetLastResponse() = 0;

        /*
         * Get the timestamp when the connection was first made.
         */
        virtual std::uint64_t GetConnectionStartTime() = 0;

        /*
         * Get the IP address of this client.
         */
        virtual std::string GetIp() = 0;

        /*
        * Send a packet to the client.
        */
        virtual void SendPacket(const IPacket& a_Data, size_t a_Size) = 0;

        /*
         * Send a packet to the client with type awareness.
         */
        template<typename T>
        void SendTypedPacket(const T& a_Data)
        {
            static_assert(std::is_base_of_v<IPacket, T>, "Can only send templated packet with a class derived from Packet.");
            SendPacket(a_Data, sizeof(T));
        }
    };
}
