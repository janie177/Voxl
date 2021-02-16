#pragma once
#include <enet/enet.h>
#include <IClientConnection.h>

namespace voxl
{
    class ClientConnection : public IClientConnection
    {
    public:
        explicit ClientConnection(ENetPeer* a_Peer);

    public:
        void Disconnect() override;
        ConnectionState GetConnectionState() override;
        std::uint64_t GetLastResponse() override;
        std::uint64_t GetConnectionStartTime() override;
        std::string GetIp() override;
        void SendPacket(Packet& a_Data, size_t a_Size) override;

    public:
        ENetPeer* GetPeer() const;

    private:
        ENetPeer* m_Peer;
        std::uint64_t m_FirstConnected;
        ConnectionState m_State;
    };
}


