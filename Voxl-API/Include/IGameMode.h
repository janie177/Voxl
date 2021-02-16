#pragma once
#include <memory>
#include <string>

namespace voxl
{
    class IWorld;

    class IGameMode
    {
    public:
        virtual ~IGameMode() = default;

        /*
         * Create a clone of this GameMode.
         */
        virtual std::unique_ptr<IGameMode> Clone() = 0;

        /*
         * Tick the gamemode and update all gameplay logic.
         */
        virtual void Tick(double a_DeltaTime, IWorld& a_World) = 0;

        /*
         * Save data related to the game-mode.
         */
        virtual void Save(IWorld& a_World) = 0;

        /*
         * Load data for the game-mode.
         */
        virtual void Load(IWorld& a_World) = 0;

        /*
         * Get the name for this game-mode.
         */
        virtual std::string GetName() const = 0;
    };
}