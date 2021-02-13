#pragma once

namespace voxl
{
    class IWorld;

    class IGameMode
    {
    public:
        virtual ~IGameMode() = 0;

        /*
         * Tick the gamemode and update all gameplay logic.
         */
        virtual void Tick(double a_DeltaTime, IWorld& a_World) = 0;
    };
}