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

    std::vector<IClientConnection*> ConnectionManager::GetConnectedClients()
    {
        std::vector<IClientConnection*> vector;
        vector.reserve(m_Clients.size());
        for (auto& entry : m_Clients)
        {
            vector.push_back(entry.second.get());
        }
        return vector;
    }

    void ConnectionManager::ProcessClientConnections()
    {
        //Process events for each client.
        ENetEvent event;
        while (enet_host_service(m_Server, &event, 0) > 0)
        {
            switch (event.type)
            {
                //Peer connected but no information is known yet so do nothing.
            case ENET_EVENT_TYPE_CONNECT:
                {
                    enet_packet_destroy(event.packet);
                }
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                {
                    IPacket* packet = reinterpret_cast<IPacket*>(event.packet->data);

                    //Packet received for user that has authenticated.
                    if (event.peer->data != nullptr)
                    {
                        ClientConnection* c = static_cast<ClientConnection*>(event.peer->data);
                        m_PacketManager->Resolve(packet->type, *packet, c);
                    }
                    else
                    {
                        //If no authentication happened, and this is not an authentication packet or the authentication failed, return.
                        if (packet->type != PacketType::AUTHENTICATE)
                        {
                            enet_peer_disconnect(event.peer, 0);
                        }
                        //If this is the authentication packet, verify the user and then add them to the active connections.
                        else
                        {
                            //Authenticate the user which adds them to the connections list. If the packet returns true it means authentication succeeded.
                            //In that case connection will contain the right data.
                            std::unique_ptr<ClientConnection> connection = std::make_unique<ClientConnection>(event.peer);
                            if(m_PacketManager->Resolve(PacketType::AUTHENTICATE, *packet, connection.get()))
                            {
                                m_Clients.insert(std::make_pair(connection->GetUsername(), std::move(connection)));
                            }
                            else
                            {
                                //Client already connected with that name or other failure.
                                enet_peer_disconnect(event.peer, 0);
                            }
                        }
                    }
                    enet_packet_destroy(event.packet);
                }
                break;
                //Peer disconnected. 
            case ENET_EVENT_TYPE_DISCONNECT:
                {
                if (event.peer->data != nullptr)
                {
                    //If this connection had a user attached to it, remove it.
                    const ClientConnection* c = static_cast<ClientConnection*>(event.peer->data);
                    const auto username = c->GetUsername();
                    if (!username.empty())
                    {
                        auto found = m_Clients.find(username);
                        if (found != m_Clients.end())
                        {
                            m_Clients.erase(found);
                        }
                    }
                    event.peer->data = nullptr;
                    enet_packet_destroy(event.packet);
                }
                }
                break;
            default:
                break;
            }
        }

        //Update client state stuff.
        for (auto& client : m_Clients)
        {
            //Clients that are marked for disconnect are removed.
            if (client.second->GetConnectionState() == ConnectionState::DISCONNECTED)
            {
                const ClientConnection* c = static_cast<ClientConnection*>(client.second.get());
                enet_peer_disconnect_now(c->GetPeer(), 0);
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
            client.second->Disconnect();
        }
    }

    IClientConnection* ConnectionManager::GetClient(const std::string& a_Username)
    {
        auto found = m_Clients.find(a_Username);
        if(found == m_Clients.end())
        {
            return nullptr;
        }
        return found->second.get();
    }
}
