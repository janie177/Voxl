#pragma once
#include "IPacketHandler.h"
#include "PacketType.h"

namespace voxl
{
    class PacketHandler_ChunkUnsubscribe : public PacketHandler<Packet_ChunkUnsubscribe>
    {
    public:
        bool OnResolve(Packet_ChunkUnsubscribe& a_Data, IConnection* a_Sender) override;
    };
}
