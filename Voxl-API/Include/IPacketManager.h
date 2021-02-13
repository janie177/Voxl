#pragma once
#include <cinttypes>

namespace voxl
{
    enum class PacketType;
    class IPacketHandler;

    class IPacketManager
    {
    public:
        virtual ~IPacketManager() = 0;

        /*
         * Resolve a packet.
         * This looks up the PacketHandler instance that was registered for this packet type.
         */
        virtual void Resolve(PacketType a_Type, void* a_Data) = 0;

        /*
         * Register a specific packet to be handled by a specific class.
         */
        virtual void Register(PacketType a_Type, IPacketHandler* a_Handler) = 0;
    };
}