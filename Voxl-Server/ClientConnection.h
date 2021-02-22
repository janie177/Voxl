#pragma once
#include <enet/enet.h>
#include <memory>
#include <IClientConnection.h>

namespace voxl
{
    class IEntityController;
    class PlayerController;

    /*
     * This class describes a connection between client and server.
     */
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
        /*
         * Get the ENet data structure containing information about this connected client.
         */
        ENetPeer* GetPeer() const;

        /*
         * Set this clients username.
         */
        void SetUsername(const std::string& a_Name);

        /*
         * Get the controller that is currently being controller by this connection.
         * If no controller is set, returns nullptr.
         */
        std::shared_ptr<PlayerController> GetController();

        /*
         * Set the controller for this connection.
         */
        void SetController(std::shared_ptr<PlayerController>& a_Controller);

    private:
        ENetPeer* m_Peer;
        std::string m_Username;
        std::uint64_t m_FirstConnected;
        ConnectionState m_State;

        //The controller that is currently being controlled by this connection.
        std::shared_ptr<PlayerController> m_Controller;
    };
}


