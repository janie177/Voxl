#pragma once
#include <IServerConnection.h>

namespace voxl
{
    class ServerConnection :  public IServerConnection
    {
    public:
        void Disconnect() override;
        ConnectionState GetConnectionState() override;
        std::uint64_t GetLastResponse() override;
        std::uint64_t GetConnectionStartTime() override;
        std::string GetIp() override;
        void SendPacket(IPacket& a_Data, size_t a_Size) override;
        void Connect(const std::string& a_Ip) override;
        IPacketManager& GetPacketManager() override;
        void ProcessPackets() override;
    };
}
