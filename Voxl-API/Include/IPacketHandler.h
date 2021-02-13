#pragma once

namespace voxl
{
    struct Packet;

    /*
     * Class that takes packets with a specific ID and handles them.
     */
    class IPacketHandler
    {
    public:
        virtual ~IPacketHandler() = 0;

        /*
         * Resolve a packet.
         */
        virtual void Resolve(Packet& a_Data) = 0;
    };

    /*
     * PacketHandler with type to make processing easier.
     */
    template<typename T>
    class PacketHandler : public IPacketHandler
    {
    public:
        /*
         * Resolve a packet of a specific type.
         */
        virtual void OnResolve(T& a_Data) = 0;

        /*
         * Internal conversion function.
         */
        void Resolve(Packet& a_Data) final override
        {
            OnResolve(static_cast<T*>(a_Data));
        }
    };
}