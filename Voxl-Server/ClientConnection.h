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
        void SendPacket(const IPacket& a_Data, size_t a_Size) override;
        std::string GetUsername() const override;

    public:
        ENetPeer* GetPeer() const;

        /*
         * Set this clients username.
         */
        void SetUsername(const std::string& a_Name);

    private:
        ENetPeer* m_Peer;
        std::string m_Username;
        std::uint64_t m_FirstConnected;
        ConnectionState m_State;
    };
}


