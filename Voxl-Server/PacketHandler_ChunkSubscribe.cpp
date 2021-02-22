#include "PacketHandler_ChunkSubscribe.h"

namespace voxl
{
    bool PacketHandler_ChunkSubscribe::OnResolve(Packet_ChunkSubscribe& a_Data, IConnection* a_Sender)
    {
        //TODO get the world the player is in, then add them tothe chunk as subscribed.
    }
}
