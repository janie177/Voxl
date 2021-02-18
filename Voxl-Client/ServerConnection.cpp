#include "ServerConnection.h"

#include <cassert>
#include <chrono>

#include "ClientPacketManager.h"

namespace voxl
{
    ServerConnection::ServerConnection(std::uint32_t a_TimeOutMillis) : m_Server(nullptr), m_Client(nullptr), m_State(ConnectionState::DISCONNECTED), m_StartTime(0), m_TimeOutMillis(a_TimeOutMillis)
    {

    }

    ServerConnection::~ServerConnection()
    {
        if(m_State != ConnectionState::DISCONNECTED)
        {
            enet_peer_disconnect_now(m_Server, 0);
            Shutdown();
        }
    }

    bool ServerConnection::Setup()
    {
        if (enet_initialize() != 0)
        {
            return false;
        }

        m_Client = enet_host_create(NULL /* create a client host */,
            1 /* only allow 1 outgoing connection */,
            2 /* allow up 2 channels to be used, 0 and 1 */,
            0 /* assume any amount of incoming bandwidth */,
            0 /* assume any amount of outgoing bandwidth */);
        if (m_Client == NULL)
        {
            return false;
        }

        m_PacketManager = std::make_unique<ClientPacketManager>();
        return true;
    }

    void ServerConnection::Shutdown()
    {
        enet_host_destroy(m_Client);
        m_Client = nullptr;
        m_PacketManager = nullptr;
        m_Server = nullptr;
        m_State = ConnectionState::DISCONNECTED;
        atexit(enet_deinitialize);
    }

    void ServerConnection::Disconnect()
    {
        assert(m_State == ConnectionState::CONNECTED && "Cannot disconnect when not connected in the first place.");
        enet_peer_disconnect_now(m_Server, 0);
        m_State = ConnectionState::DISCONNECTED;
    }

    ConnectionState ServerConnection::GetConnectionState()
    {
        return m_State;
    }

    std::uint64_t ServerConnection::GetLastResponse()
    {
        if (m_Server == nullptr) return 0;
        return m_Server->lastReceiveTime;
    }

    std::uint64_t ServerConnection::GetConnectionStartTime()
    {
        return m_StartTime;
    }

    std::string ServerConnection::GetIp()
    {
        char ip[255];
        enet_address_get_host_ip(&m_Server->address, ip, 255);
        return std::string(ip);
    }

    void ServerConnection::SendPacket(const IPacket& a_Data, size_t a_Size)
    {
        //Send the packet if connected still.
        if (m_State == ConnectionState::CONNECTED)
        {
            ENetPacket* packet = enet_packet_create(&a_Data, a_Size, ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(m_Server, 0, packet);
        }
    }

    bool ServerConnection::Connect(const std::string& a_Ip, std::uint32_t a_Port, const Packet_Authenticate& a_Authentication)
    {
        assert(m_Client != nullptr && "Client host not initialized! Call Setup before trying to connect to a server.");
        assert(m_State == ConnectionState::DISCONNECTED && "Can't attempt connection when already connected.");

        //Mark as connecting.
        m_State = ConnectionState::CONNECTING;

        ENetAddress address;
        ENetEvent event;
        enet_address_set_host(&address, a_Ip.c_str());
        address.port = a_Port;

        m_Server = enet_host_connect(m_Client, &address, 2, 0);
        /* Wait up to 5 seconds for the connection attempt to succeed. */
        if (m_Server != NULL && (enet_host_service(m_Client, &event, m_TimeOutMillis) > 0 && event.type == ENET_EVENT_TYPE_CONNECT))
        {
            m_StartTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();

            //Authenticate with the authentication packet.
            ENetPacket* packet = enet_packet_create(&a_Authentication, sizeof(Packet_Authenticate), ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(m_Server, 0, packet);

            //Wait for a response packet.
            ENetEvent authenticationResponse;
            if(enet_host_service(m_Client, &authenticationResponse, m_TimeOutMillis) && authenticationResponse.type == ENET_EVENT_TYPE_RECEIVE)
            {
                //See if the authentication succeeded or not.
                IPacket* responsePacket = reinterpret_cast<IPacket*>(authenticationResponse.packet->data);
                if (responsePacket->type == PacketType::AUTHENTICATION_RESPONSE && static_cast<Packet_AuthenticationResponse*>(responsePacket)->accepted)
                {
                    m_State = ConnectionState::CONNECTED;
                    return true;
                }
            }

            m_State = ConnectionState::DISCONNECTED;
            return false;
        }
        else
        {
            /* Either the 5 seconds are up or a disconnect event was */
            /* received. Reset the peer in the event the 5 seconds   */
            /* had run out without any significant event.            */
            enet_peer_reset(m_Server);
            m_State = ConnectionState::DISCONNECTED;
            return false;
        }
    }

    IPacketManager& ServerConnection::GetPacketManager()
    {
        assert(m_PacketManager && "Packet manager not initialized.");
        return *m_PacketManager;
    }

    void ServerConnection::ProcessPackets()
    {
        //Process events for each client.
        ENetEvent event;
        while (enet_host_service(m_Client, &event, 0) > 0)
        {
            //Only allow events from the server.
            if(event.peer != m_Server)
            {
                return;
            }

            switch (event.type)
            {
                //Servers don't connect to players?
            case ENET_EVENT_TYPE_CONNECT:
            {
                enet_packet_destroy(event.packet);
            }
            break;
            case ENET_EVENT_TYPE_RECEIVE:
            {
                IPacket* packet = reinterpret_cast<IPacket*>(event.packet->data);

                m_PacketManager->Resolve(packet->type, *packet, this);
                enet_packet_destroy(event.packet);
            }
            break;
            //Server has disconnected so I guess this is it :(
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                Disconnect();
                enet_packet_destroy(event.packet);
            }
            break;
            default:
                break;
            }
        }
    }
}
