#include "PacketHandler_VoxelUpdate.h"

namespace voxl
{
    bool PacketHandler_VoxelUpdate::OnResolve(Packet_VoxelUpdate& a_Data, IConnection* a_Sender)
    {
        //TODO get the block in the world the player is in, and then check if the player can break it (nearby, no cooldown etc).
        //TODO then send voxel update back to player if undone or send to everyone else when approved.
        return true;
    }
}
