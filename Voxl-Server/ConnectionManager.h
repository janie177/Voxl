#pragma once
#include <IConnectionManager.h>
#include <map>
#include <string>
#include <thread>
#include <unordered_map>


struct _ENetHost;

namespace voxl
{
    class ConnectionManager : public IConnectionManager
    {
    public:
        ConnectionManager();

        bool Start(const ServerSettings& a_Settings) override;
        void Stop() override;
        std::vector<IClientConnection*> GetConnectedClients() override;
        void ProcessClientConnections() override;
        IPacketManager& GetPacketManager() override;
        void DisconnectClients() override;
        IClientConnection* GetClient(const std::string& a_Username) override;
    private:
        _ENetHost* m_Server;
        std::unordered_map<std::string, std::unique_ptr<IClientConnection>> m_Clients;
        std::unique_ptr<IPacketManager> m_PacketManager;
    };
}

