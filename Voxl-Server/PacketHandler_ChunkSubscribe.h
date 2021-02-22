#pragma once
#include "IPacketHandler.h"
#include "PacketType.h"

namespace voxl
{
    class PacketHandler_ChunkSubscribe : public PacketHandler<Packet_ChunkSubscribe>
    {
    public:
        bool OnResolve(Packet_ChunkSubscribe& a_Data, IConnection* a_Sender) override;
    };
}
