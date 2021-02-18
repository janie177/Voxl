#include "ClientPacketManager.h"


#include <iostream>
#include <string>


#include "IPacketHandler.h"
#include "PacketType.h"

namespace voxl
{
    ClientPacketManager::ClientPacketManager()
    {
        //Resize to fit one handler for each packet type.
        m_Handlers.resize(static_cast<size_t>(PacketType::UNKNOWN) + 1);
    }

    bool ClientPacketManager::Resolve(PacketType a_Type, IPacket& a_Data, IConnection* a_Sender)
    {
        auto* found = m_Handlers[static_cast<size_t>(a_Type)].get();
        if (found != nullptr)
        {
            return found->Resolve(a_Data, a_Sender);
        }

        std::cout << "Warning: Packet of type " << std::to_string((int)a_Type) << " is not handled." << std::endl;

        return false;
    }

    void ClientPacketManager::Register(PacketType a_Type, std::unique_ptr<IPacketHandler>&& a_Handler)
    {
        m_Handlers[static_cast<size_t>(a_Type)] = std::move(a_Handler);
    }
}
