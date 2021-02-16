#pragma once
#include <vector>

namespace voxl
{
    class IClientConnection;
    class IPacketManager;
    struct ServerSettings;

    class IConnectionManager
    {
    public:
        virtual ~IConnectionManager() = default;

        /*
         * Setup the server to accept connections.
         */
        virtual bool Start(const ServerSettings& a_Settings) = 0;
        
        /*
         * Stop the networking server.
         */
        virtual void Stop() = 0;

        /*
         * Get all currently connected clients.
         */
        virtual std::vector<IClientConnection*>& GetConnectedClients() = 0;

        /*
         * Process all connected clients.
         */
        virtual void ProcessClientConnections() = 0;

        /*
         * Get the packet manager used to handle incoming packets.
         */
        virtual IPacketManager& GetPacketManager() = 0;

        /*
         * Disconnect all clients.
         */
        virtual void DisconnectClients() = 0;
    };
}
