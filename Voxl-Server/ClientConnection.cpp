#include "ClientConnection.h"

#include <chrono>
#include <cassert>

namespace voxl
{
    ClientConnection::ClientConnection(ENetPeer* a_Peer) : m_Peer(a_Peer), m_FirstConnected(0)
    {
        assert(a_Peer != nullptr);
        m_FirstConnected = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        m_State = ConnectionState::CONNECTED;
    }

    void ClientConnection::Disconnect()
    {
        m_State = ConnectionState::DISCONNECTED;
    }

    ConnectionState ClientConnection::GetConnectionState()
    {
        return m_State;
    }

    std::uint64_t ClientConnection::GetLastResponse()
    {
        return m_Peer->lastReceiveTime;
    }

    std::uint64_t ClientConnection::GetConnectionStartTime()
    {
        return m_FirstConnected;
    }

    std::string ClientConnection::GetIp()
    {
        char name[255];
        enet_address_get_host_ip(&(m_Peer->address), name, 255);
        return std::string(name);
    }

    void ClientConnection::SendPacket(const IPacket& a_Data, size_t a_Size)
    {
        //Send the packet if connected still.
        if(m_State == ConnectionState::CONNECTED)
        {
            ENetPacket* packet = enet_packet_create(&a_Data, a_Size, ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(m_Peer, 0, packet);
        }
    }

    std::string ClientConnection::GetUsername() const
    {
        return m_Username;
    }

    ENetPeer* ClientConnection::GetPeer() const
    {
        return m_Peer;
    }

    void ClientConnection::SetUsername(const std::string& a_Name)
    {
        assert(a_Name.length() <= 255);
        m_Username = a_Name;
    }
}
