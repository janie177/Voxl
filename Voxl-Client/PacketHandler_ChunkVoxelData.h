#pragma once
#include "IPacketHandler.h"
#include "PacketType.h"

namespace voxl
{
    class PacketHandler_ChunkVoxelData : public PacketHandler<Packet_ChunkVoxelData>
    {
    public:
        bool OnResolve(Packet_ChunkVoxelData& a_Data, IConnection* a_Sender) override;
    };
}
