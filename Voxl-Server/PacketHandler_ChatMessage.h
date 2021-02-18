#pragma once
#include <string>

#include "IPacketHandler.h"
#include "PacketType.h"
#include "ConnectionManager.h"
#include "ClientConnection.h"
#include "logging/Logger.h"
#include "other/ServiceLocator.h"

namespace voxl
{
    class ConnectionManager;

    class PacketHandler_ChatMessage : public PacketHandler<Packet_ChatMessage>
    {
    public:
        PacketHandler_ChatMessage(ConnectionManager& a_Manager) : m_Manager(a_Manager)
        {

        }

        bool OnResolve(Packet_ChatMessage& a_Data, IConnection* a_Sender) override
        {
            //Copy the packet and fill in the username. Then send it to everyone.
            Packet_ChatMessage out = a_Data;
            IClientConnection* client = static_cast<IClientConnection*>(a_Sender);
            strcpy_s(out.sender, client->GetUsername().size() + 1, client->GetUsername().c_str());

            for(auto& c : m_Manager.GetConnectedClients())
            {
                c->SendTypedPacket(out);
            }

            //Log the message
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Info, client->GetUsername() + ": " + a_Data.message);

            return true;
        }

    private:
        ConnectionManager& m_Manager;
    };
}
