#pragma once
#include <EntityType.h>
#include <IEntity.h>

#include "PlayerController.h"

namespace voxl
{
    class ClientConnection;
    class World;

    class Player : public IPlayer
    {
    public:
        Player(World* a_World, std::shared_ptr<PlayerController> a_Controller);

        utilities::Transform& GetTransform() final override;
        EntityType GetType() const final override;

    private:
        //The players position, orientation and scale.
        utilities::Transform m_Transform;

        //The connection in control of this player.
        ClientConnection* m_Connection;

        //The world the player is in.
        World* m_World;

        //Whether or not this player is currently active.
        //This is set to false when a controller stops using a player.
        bool m_Active;
    };
}
