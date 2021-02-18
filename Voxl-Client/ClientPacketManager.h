#pragma once
#include <IPacketManager.h>
#include <unordered_map>

namespace voxl
{
    class ClientPacketManager : public IPacketManager
    {
    public:
        ClientPacketManager();

        bool Resolve(PacketType a_Type, IPacket& a_Data, IConnection* a_Sender) override;
        void Register(PacketType a_Type, std::unique_ptr<IPacketHandler>&& a_Handler) override;
    private:
        //Map containing all the registered packet handlers.
        std::vector<std::unique_ptr<IPacketHandler>> m_Handlers;
    };
}