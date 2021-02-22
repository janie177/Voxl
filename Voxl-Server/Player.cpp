#include "Player.h"
#include <cassert>

#include "other/Transform.h"

namespace voxl
{
    Player::Player(ClientConnection* a_Connection) : m_Connection(a_Connection), m_Active(true)
    {
        assert(a_Connection != nullptr);
    }

    utilities::Transform& Player::GetTransform()
    {
        return m_Transform;
    }

    EntityType Player::GetType() const
    {
        return EntityType::PLAYER;
    }
}
