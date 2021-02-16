#pragma once
#include <cinttypes>
#include <memory>

namespace voxl
{
    enum class PacketType;
    class IPacketHandler;
    class IConnection;
    struct IPacket;

    class IPacketManager
    {
    public:
        virtual ~IPacketManager() = default;

        /*
         * Resolve a packet.
         * This looks up the PacketHandler instance that was registered for this packet type.
         * When no sender is available, nullptr can be passed.
         */
        virtual bool Resolve(PacketType a_Type, IPacket& a_Data, IConnection* a_Sender) = 0;

        /*
         * Register a specific packet to be handled by a specific class.
         */
        virtual void Register(PacketType a_Type, std::unique_ptr<IPacketHandler>&& a_Handler) = 0;
    };
}