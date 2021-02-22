#pragma once

namespace voxl
{
    class IEntity;

    /*
     * Base controller interface.
     */
    class IEntityController
    {
    public:
        virtual ~IEntityController() = default;

        /*
         * Tick this controller and update the controlled entity.
         */
        virtual void Tick(float a_DeltaTime) = 0;

        /*
         * Get the entity being controller.
         */
        virtual IEntity* GetEntity() = 0;

        /*
         * Set the entity to be controller by this controller.
         */
        virtual void SetEntity(IEntity* a_Entity) = 0;
    };

    
}