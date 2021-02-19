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

    class PacketHandler_Authenticate : public PacketHandler<Packet_Authenticate>
    {
    public:
        PacketHandler_Authenticate(ConnectionManager& a_Manager) : m_Manager(a_Manager)
        {
            
        }

        bool OnResolve(Packet_Authenticate& a_Data, IConnection* a_Sender) override
        {
            //Cast to implementation type.
            ClientConnection* sender = static_cast<ClientConnection*>(a_Sender);

            //The response packet.
            Packet_AuthenticationResponse response;

            //Retrieve the username and try to authenticate. Ensure not already logged in.
            const std::string name = a_Data.name;

            //TODO authenticate
            bool credentialsValid = true;

            //Already connected.
            if(m_Manager.GetClient(name) != nullptr || !credentialsValid)
            {
                response.accepted = false;
                sender->SendTypedPacket(response);

                //Log the request
                utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Info, "Authentication request from: " + a_Sender->GetIp() + " for user " + sender->GetUsername() + " denied.");
                return false;
            }
            else
            {
                //Not connected and authentication checks out. Allow connection.
                response.accepted = true;
            }

            //Set the username.
            sender->SetUsername(name);

            //Send a confirmation to the client.
            sender->SendTypedPacket(response);

            //Log the request
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Info, "Authentication request from: " + a_Sender->GetIp() + " for user " + sender->GetUsername() + " approved.");
            return true;
        }

    private:
        ConnectionManager& m_Manager;
    };
}
