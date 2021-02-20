#pragma once
#include <utility>


#include "IPacketHandler.h"
#include "PacketType.h"
#include "ConnectionManager.h"
#include "ClientConnection.h"

namespace voxl
{
    class ConnectionManager;

    class PacketHandler_Request : public PacketHandler<Packet_Request>
    {
    public:
        PacketHandler_Request(const std::vector<char>& a_VoxelInfoFile)
        {
            //Set up the memory so that the packet si directly followed by the file data.
            m_VoxelInfoPacket.resize(a_VoxelInfoFile.size() + sizeof(Packet_VoxelInfo));
            Packet_VoxelInfo* ptr = reinterpret_cast<Packet_VoxelInfo*>(&m_VoxelInfoPacket[0]);
            ptr->size = a_VoxelInfoFile.size();
            Packet_VoxelInfo toCopy;
            memcpy_s(ptr, sizeof(Packet_VoxelInfo), &toCopy, sizeof(Packet_VoxelInfo));

            m_VoxelInfoPacket.insert(m_VoxelInfoPacket.begin() + sizeof(Packet_VoxelInfo), a_VoxelInfoFile.begin(), a_VoxelInfoFile.end());
            
        }

        bool OnResolve(Packet_Request& a_Data, IConnection* a_Sender) override
        {
            if(a_Sender != nullptr)
            {
                //Voxel info packet has been requested by the sender, so send it.
                if(a_Data.requested == PacketType::VOXEL_INFO)
                {
                    IPacket* packetPtr = reinterpret_cast<Packet_VoxelInfo*>(&m_VoxelInfoPacket[0]);
                    a_Sender->SendPacket(*packetPtr, m_VoxelInfoPacket.size());
                }
            }

            return true;
        }

    private:
        std::vector<char> m_VoxelInfoPacket;
    };
}
