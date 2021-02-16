#pragma once

namespace voxl
{
    struct IPacket;
    class IConnection;

    /*
     * Class that takes packets with a specific ID and handles them.
     */
    class IPacketHandler
    {
    public:
        virtual ~IPacketHandler() = default;

        /*
         * Resolve a packet.
         */
        virtual bool Resolve(IPacket& a_Data, IConnection* a_Sender) = 0;
    };

    /*
     * PacketHandler with type to make processing easier.
     */
    template<typename T>
    class PacketHandler : public IPacketHandler
    {
        static_assert(std::is_base_of_v<IPacket, T>, "PacketHandler required template type that is derived from (IPacket.");
    public:
        /*
         * Resolve a packet of a specific type.
         */
        virtual bool OnResolve(T& a_Data, IConnection* a_Sender) = 0;

        /*
         * Internal conversion function.
         */
        bool Resolve(IPacket& a_Data, IConnection* a_Sender) final override
        {
            auto* derived = static_cast<T*>(&a_Data);
            return OnResolve(*derived, a_Sender);
        }
    };
}