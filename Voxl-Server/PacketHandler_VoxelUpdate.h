#pragma once
#include "IPacketHandler.h"
#include "PacketType.h"

namespace voxl
{
    class PacketHandler_VoxelUpdate : public PacketHandler<Packet_VoxelUpdate>
    {
    public:
        bool OnResolve(Packet_VoxelUpdate& a_Data, IConnection* a_Sender) override;
    };
}
