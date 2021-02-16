#include "PacketManager.h"

#include <cassert>

#include "IPacketHandler.h"
#include "PacketType.h"

namespace voxl
{
    PacketManager::PacketManager()
    {
        //Resize to fit one handler for each packet type.
        m_Handlers.resize(static_cast<size_t>(PacketType::UNKNOWN) + 1);
    }

    bool PacketManager::Resolve(PacketType a_Type, IPacket& a_Data, IConnection* a_Sender)
    {
        auto* found = m_Handlers[static_cast<size_t>(a_Type)].get();
        if(found != nullptr)
        {
            return found->Resolve(a_Data, a_Sender);
        }
        return false;
    }

    void PacketManager::Register(PacketType a_Type, std::unique_ptr<IPacketHandler>&& a_Handler)
    {
        m_Handlers[static_cast<size_t>(a_Type)] = std::move(a_Handler);
    }
}
