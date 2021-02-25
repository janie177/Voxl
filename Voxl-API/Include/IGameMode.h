#pragma once
#include <memory>
#include <string>

#include "ClientRules.h"

namespace voxl
{
    class IWorld;
    class IGame;
    class VoxelRegistry;
    class EntityRegistry;
    class IClientConnection;

    /*
     * A gamemode makes up the rules about how clients, blocks and entities behave.
     * It defines all gameplay elements.
     */
    class IGameMode
    {
    public:
        virtual ~IGameMode() = default;

        /*
         * Clone this gamemode for the given IGame instance.
         */
        virtual std::shared_ptr<IGameMode> Clone(IGame& a_Game) = 0;

        /*
         * Get the rules for this gamemode for the given player.
         */
        virtual ClientRules GetRules(IClientConnection& a_Client) = 0;

        /*
         * The a reference to the voxel registry belonging to this gamemode.
         */
        virtual VoxelRegistry& GetVoxelRegistry() = 0;

        /*
         * The a reference to the entity registry belonging to this gamemode.
         */
        virtual EntityRegistry& GetEntityRegistry() = 0;

        /*
         * Add a client to this gamemode.
         */
        virtual void CreatePlayer(IClientConnection& a_Client) = 0;

        /*
         * Remove a client from this gamemode.
         * This should be called when a player quits the game.
         */
        virtual void RemovePlayer(IClientConnection& a_Client) = 0;

        /*
         * Get the game that this gamemode belongs to.
         */
        virtual IGame& GetGame() = 0;

        /*
         * Tick the gamemode and update all gameplay logic.
         */
        virtual void Tick(double a_DeltaTime) = 0;

        /*
         * Save data related to the game-mode.
         */
        virtual void Save() = 0;

        /*
         * Load data for the game-mode.
         */
        virtual void Load() = 0;

        /*
         * Get the name for this game-mode.
         */
        virtual std::string GetName() const = 0;

        /*
         * Shut down this gamemode.
         */
        virtual void Unload() = 0;

        /*
         * Try to load the voxel data file.
         * If no file was exists, returns false. True if file loaded successfully.
         */
        virtual bool LoadVoxelTypesFile() = 0;

        /*
         * Create the default voxel data file.
         */
        virtual void CreateVoxelTypesFile() = 0;
    };
}
