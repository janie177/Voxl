#pragma once
#include <IPacketManager.h>

namespace voxl
{
    class PacketManager : public IPacketManager
    {
    public:
        void Register(PacketType a_Type, IPacketHandler* a_Handler) override;
        void Resolve(PacketType a_Type, void* a_Data, IConnection* a_Sender) override;
    };
};