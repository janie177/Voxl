#pragma once

namespace voxl
{
    /*
     * All packet types with their internal IDs.
     */
    enum class PacketType
    {
        UNKNOWN,
        DUMMY
    };

    /*
     * Base packet class used for polymorphic passing in functions.
     */
    struct Packet
    {
        Packet(PacketType a_Type) : type(a_Type) {}
        PacketType type;
    };

    /*
     * Typed packet class used as base and to statically link type and enumeration.
     */
    template<PacketType T>
    struct PacketBase : public Packet
    {
        PacketBase() : Packet(T) {}
    };

    /*
     * Dummy packet.
     */
    struct DummyPacket : public PacketBase<PacketType::DUMMY>
    {
        int test[10]; //10 test ints.
        char chars[4]{'T', 'e', 's', 't'};
    };
}