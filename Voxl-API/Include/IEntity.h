#pragma once
#include <memory>


#include "EntityType.h"
#include "other/Transform.h"

namespace voxl
{
    class IWorld;
    class IChunk;
    class IEntityController;

    /*
     * Base entity class.
     * All entities have this in common.
     */
    class IEntity
    {
    public:
        virtual ~IEntity() = default;

        /*
         * Set the controller for this entity.
         */
        virtual void SetController(std::shared_ptr<IEntityController>& a_Controller) = 0;

        /*
         * Get the controller for this entity.
         */
        virtual std::shared_ptr<IEntityController> GetController() = 0;

        /*
         * Get this entities transform.
         */
        virtual utilities::Transform& GetTransform() = 0;

        /*
         * Get the type of this entity.
         */
        virtual EntityType GetType() const = 0;

        /*
         * Get the world this entity is in.
         * Returns nullptr if the world is invalid.
         */
        virtual IWorld* GetWorld() const = 0;

        /*
         * Get the chunk this entity is in.
         * Returns nullptr if the chunk is invalid.
         */
        virtual IChunk* GetChunk() const = 0;

        /*
         * Tick this entity.
         * This internally updates the controller if present.
         */
        virtual void Tick(float a_DeltaTime) = 0;

        /*
         * Get the unique ID for this entity.
         */
        virtual std::uint64_t GetUniqueId() = 0;

        /*
         * Mark this entity for delete.
         * This will remove the entity from the world next update cycle.
         */
        virtual void Destroy() = 0;

        /*
         * Returns true if this entity will be removed next game cycle.
         */
        virtual bool IsMarkedForDestroy() = 0;
    };

    /*
     * Basic player type.
     */
    class IPlayer : public IEntity
    {
    public:
        //TODO unique things that players have both on the server and client.
    };
}
