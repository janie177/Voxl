#include "ConnectionManager.h"
#include <enet/enet.h>
#include <IServer.h>
#include <IClientConnection.h>

#include "ClientConnection.h"
#include "PacketManager.h"

namespace voxl
{
    ConnectionManager::ConnectionManager() : m_Server(nullptr)
    {

    }

    bool ConnectionManager::Start(const ServerSettings& a_Settings)
    {
        //Start the networking.
        if (enet_initialize() != 0)
        {
            return false;
        }

        //Set up the ENet server.
        {
            ENetAddress address;
            ENetHost* server;
            enet_address_set_host(&address, a_Settings.ip.c_str());
            address.port = static_cast<std::uint16_t>(a_Settings.port);

            server = enet_host_create(&address     /* the address to bind the server host to */,
                a_Settings.maximumConnections      /* allow up to 32 clients and/or outgoing connections */,
                2      /* allow up to 2 channels to be used, 0 and 1 */,
                0      /* assume any amount of incoming bandwidth */,
                0      /* assume any amount of outgoing bandwidth */);

            if (server == NULL)
            {
                return false;
            }

            //Store pointer for deletion and access.
            m_Server = server;

            //Creat the packet manager instance.
            m_PacketManager = std::make_unique<PacketManager>();
        }

        return true;
    }

    void ConnectionManager::Stop()
    {
        //Shut down ENet.
        enet_host_destroy(m_Server);
        enet_deinitialize();
    }

    std::vector<IClientConnection*>& ConnectionManager::GetConnectedClients()
    {
        return m_Clients;
    }

    void ConnectionManager::ProcessClientConnections()
    {
        ENetEvent event;

        //Update the clients
        for (auto& client : m_Clients)
        {
            ClientConnection* c = static_cast<ClientConnection*>(client);
            //Process events for each client.
            while (enet_host_service(m_Server, &event, 1000) > 0)
            {
                switch (event.type)
                {
                case ENET_EVENT_TYPE_CONNECT:
                    //TODO create ClientConnection and link to this thing.
                    //event.peer->data = "Client information";
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    //TODO process packet
                    enet_packet_destroy(event.packet);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    //TODO disconnect and then remove from array.
                    event.peer->data = NULL;
                default:
                    break;
                }
            }
        }
    }

    IPacketManager& ConnectionManager::GetPacketManager()
    {
        return *m_PacketManager;
    }

    void ConnectionManager::DisconnectClients()
    {
        for (auto& client : m_Clients)
        {
            client->Disconnect();
        }
    }
}
