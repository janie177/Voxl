#include "PacketHandler_ChunkUnsubscribe.h"

namespace voxl
{
    bool PacketHandler_ChunkUnsubscribe::OnResolve(Packet_ChunkUnsubscribe& a_Data, IConnection* a_Sender)
    {
        //TODO get world the player is in and remove player from that chunk.
        return true;
    }
}
