#pragma once
#include <IConnectionManager.h>
#include <thread>


struct _ENetHost;

namespace voxl
{
    class ConnectionManager : public IConnectionManager
    {
    public:
        ConnectionManager();

        bool Start(const ServerSettings& a_Settings) override;
        void Stop() override;
        std::vector<IClientConnection*>& GetConnectedClients() override;
        void ProcessClientConnections() override;
        IPacketManager& GetPacketManager() override;
        void DisconnectClients() override;

    private:
        _ENetHost* m_Server;
        std::vector<IClientConnection*> m_Clients;
        std::unique_ptr<IPacketManager> m_PacketManager;
    };
}

