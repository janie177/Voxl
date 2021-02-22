#pragma once
#include "VoxelData.h"

#include "Utility.h"

namespace voxl
{
    /*
     * All packet types with their internal IDs.
     * Important: Unknown should be the last packed and each packet should have their natural enumeration int value.
     */
    enum class PacketType
    {
        AUTHENTICATE = 0,           //Player attempting to authenticate with a server.

        REQUEST,                    //An arbitrary request that requires the server to respond in a way. Allows some small data to be transmitted and interpreted as needed.

        AUTHENTICATION_RESPONSE,    //Approval or denial by the server when a player tries authenticating.

        CHUNK_SUBSCRIBE,    //Player subscribes to a chunk.
        CHUNK_VOXEL_DATA,   //All voxel data in a chunk.
        VOXEL_UPDATE,       //A voxel change in a chunk.
        CHUNK_UNSUBSCRIBE,  //Player no longer subscribes to a chunk.

        VOXEL_INFO,         //All information about the voxels on a server.

        CHAT_MESSAGE,       //A chat message

        //UNKNOWN packet is always the last one to determine the amount of packets.
        UNKNOWN,
    };

    /*
     * Base packet class used for polymorphic passing in functions.
     */
    struct IPacket
    {
        virtual ~IPacket() = default;
        IPacket(PacketType a_Type) : type(a_Type) {}
        const PacketType type;
    };

    /*
     * Typed packet class used as base and to statically link type and enumeration.
     */
    template<PacketType T>
    struct PacketBase : public IPacket
    {
        PacketBase() : IPacket(T) {}
    };

    /*
     * The packet used for authentication.
     * This should be the first packet that is sent by the client and received by the server.
     */
    struct Packet_Authenticate : public PacketBase<PacketType::AUTHENTICATE>
    {
        char name[255];         //Username

        //TODO encryption and keys for verification.
    };

    /*
     * Sent from server to client to verify or deny authentication.
     */
    struct Packet_AuthenticationResponse : public PacketBase<PacketType::AUTHENTICATION_RESPONSE>
    {
        bool accepted;
    };

    /*
     * A chat message sent by a player to the server or from the server to all players.
     */
    struct Packet_ChatMessage : public PacketBase<PacketType::CHAT_MESSAGE>
    {
        //Timestamp at which the message was sent.
        std::uint64_t timeStamp;

        //Name of the message sender.
        char sender[255];

        //The message.
        char message[255];
    };

    /*
     * Request information of a specific type.
     */
    struct Packet_Request : public PacketBase<PacketType::REQUEST>
    {
        //The requested data packet.
        PacketType requested;

        /*
         * Information about the request, stored as several data types in a union.
         * This can be interpreted by the received however it wants.
         */
        union
        {
            float floats[16];
            int ints[16];
            short shorts[32];
            long longs[8];
            unsigned uints[16];
            unsigned short uShorts[32];
        };
    };

    /*
     * Packet containing the voxel information json file.
     */
    struct Packet_VoxelInfo : public PacketBase<PacketType::VOXEL_INFO>
    {
        /*
         * The size of the 
         */
        size_t size;
    };

    /*
     * Sent by a player to the server when they want to receive updates from a chunk.
     */
    struct Packet_ChunkSubscribe : public PacketBase<PacketType::CHUNK_SUBSCRIBE>
    {
        //The coordinates of the chunk.
        int coordinates[3];
    };

    /*
     * Sent by the player to the server when they no longer want chunk updates.
     */
    struct Packet_ChunkUnsubscribe : public PacketBase<PacketType::CHUNK_UNSUBSCRIBE>
    {
        //The coordinates of the chunk.
        int coordinates[3];
    };

    /*
     * All voxel data from within a chunk.
     */
    struct Packet_ChunkVoxelData : public PacketBase<PacketType::CHUNK_VOXEL_DATA>
    {
        //The coordinates of the chunk.
        int coordinates[3];

        //The actual chunk data.
        VoxelData data[CHUNK_SIZE_CUBED];
    };

    struct Packet_VoxelUpdate : public PacketBase<PacketType::VOXEL_UPDATE>
    {
        //The coordinates of the block.
        int coordinatesBlock[3];

        //The updated data.
        VoxelData data;
    };
}
