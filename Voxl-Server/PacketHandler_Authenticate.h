#pragma once
#include <string>

#include "IPacketHandler.h"
#include "PacketType.h"
#include "ConnectionManager.h"
#include "ClientConnection.h"

namespace voxl
{
    class ConnectionManager;

    class PacketHandler_Authenticate : public PacketHandler<Packet_Authenticate>
    {
    public:
        PacketHandler_Authenticate(ConnectionManager& a_Manager) : m_Manager(a_Manager)
        {
            
        }

        bool OnResolve(Packet_Authenticate& a_Data, IConnection* a_Sender) override
        {
            //Retrieve the username and try to authenticate. Ensure not already logged in.
            const std::string name = a_Data.name;

            //TODO authenticate

            //Already connected.
            if(m_Manager.GetClient(name) != nullptr)
            {
                return false;
            }

            //Cast to implementation type.
            ClientConnection* sender = static_cast<ClientConnection*>(a_Sender);

            //Set the username.
            sender->SetUsername(name);

            return true;
        }

    private:
        ConnectionManager& m_Manager;
    };
}
