#pragma once
#include <IServerConnection.h>
#include <enet/enet.h>
#include <memory>

namespace voxl
{
    class ServerConnection :  public IServerConnection
    {
    public:
        ServerConnection(std::uint32_t a_TimeOutMillis);

        ~ServerConnection() override;

        /*
         * Setup the connection systems.
         */
        bool Setup();

        /*
         * Shutdown connection systems.
         */
        void Shutdown();

        void Disconnect() override;
        ConnectionState GetConnectionState() override;
        std::uint64_t GetLastResponse() override;
        std::uint64_t GetConnectionStartTime() override;
        std::string GetIp() override;
        void SendPacket(const IPacket& a_Data, size_t a_Size) override;
        bool Connect(const std::string& a_Ip, std::uint32_t a_Port, const Packet_Authenticate& a_Authentication) override;
        IPacketManager& GetPacketManager() override;
        void ProcessPackets() override;
        bool WaitForPacket(PacketType a_Type, std::uint32_t a_TimeOutMillis, std::function<void(IPacket* a_Packet)> a_OnReceive) override;
    private:
        ENetPeer* m_Server;
        ENetHost* m_Client;
        ConnectionState m_State;
        std::unique_ptr<IPacketManager> m_PacketManager;
        std::uint64_t m_StartTime;

        //Millis before timing out.
        std::uint32_t m_TimeOutMillis;
    };
}
