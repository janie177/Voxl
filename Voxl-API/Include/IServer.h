#pragma once
#include <memory>
#include <string>
#include <vector>

#include "logging/Logger.h"

namespace voxl
{
    struct WorldSettings;
    class IWorldGenerator;
    class IWorld;
    class IGameMode;
    class IChunkStore;
    class VoxelRegistry;
    class IClientConnection;
    class IPacketManager;
    class IConnectionManager;

    struct ServerSettings
    {
        //The ticks per second at which the server processes information.
        std::uint32_t tps = 32;

        //The directory where worlds are stored.
        std::string worldsDirectory = "worlds";

        //Worlds that are loaded by default.
        std::vector<std::string> defaultWorlds{"world"};

        //Maximum number of connections.
        std::uint32_t maximumConnections = 128;

        //The server IP to bind to.
        std::string ip = "127.0.0.1";

        //The port to listen at.
        std::uint32_t port = 28280;
    };

    /*
     * The main server interface. Contains all server related functions and settings.
     */
    class IServer
    {
    public:
        virtual ~IServer() = default;

        /*
         * Returns true when the server has shut down.
         */
        virtual bool HasShutDown() = 0;

        /*
         * Initialize server systems. 
         */
        virtual bool Start() = 0;

        /*
         * Stop the server.
         */
        virtual void ShutDown(bool a_SaveAll) = 0;

        /*
         * Restart the server.
         */
        virtual void Restart(bool a_SaveAll) = 0;

        /*
         * Update the server, ticking and updating all worlds and game-modes.
         */
        virtual void Tick(double a_DeltaTime) = 0;

        /*
         * Try to load the server settings file.
         */
        virtual bool LoadSettingsFile() = 0;

        /*
         * Create the default server settings file.
         */
        virtual void CreateSettingsFile(const ServerSettings& a_Settings) = 0;

        /*
         * Try to load the voxel data file.
         * If no file was exists, returns false. True if file loaded successfully.
         */
        virtual bool LoadVoxelTypesFile() = 0;

        /*
         * Create the default voxel data file.
         */
        virtual void CreateVoxelTypesFile() = 0;

        /*
         * Get the world with the given name.
         * When no world was loaded for the given name, nullptr is returned.
         */
        virtual IWorld* GetWorld(const std::string& a_Name) = 0;

        /*
         * Unload a world from memory.
         */
        virtual bool UnloadWorld(const std::string& a_Name, bool a_Save) = 0;

        /*
         * Load a world from file.
         * Returns a pointer to the loaded world if existing.
         * Returns nullptr otherwise.
         */
        virtual IWorld* LoadWorld(const std::string& a_Name) = 0;

        /*
         * Returns true if the world exists on disk or in memory.
         */
        virtual bool WorldExists(const std::string& a_Name) = 0;

        /*
         * Create a new world.
         * Returns a pointer to the created world.
         * If a world with the given name already exists, returns nullptr.
         */
        virtual IWorld* CreateWorld(const WorldSettings& a_Settings) = 0;

        /*
         * Delete a world forever.
         * Returns true if the world existed and has been removed, false otherwise.
         */
        virtual bool DeleteWorld(const std::string& a_Name) = 0;

        /*
         * Get all worlds that are currently loaded.
         */
        virtual std::vector<IWorld*> GetWorlds() = 0;

        /*
         * Register a world generator with the given name.
         */
        virtual void RegisterWorldGenerator(const std::string& a_Name, std::shared_ptr<IWorldGenerator>& a_Generator) = 0;

        /*
         * Register a gamemode with the server using the given name as handle.
         */
        virtual void RegisterGameMode(const std::string& a_Name, std::unique_ptr<IGameMode>&& a_GameMode) = 0;

        /*
         * Get the world generator registered with the given name.
         * If no world generator exists with that name, nullptr is returned.
         */
        virtual std::shared_ptr<IWorldGenerator> GetWorldGenerator(const std::string& a_Name) = 0;

        /*
         * Create a gamemode instance of the gamemode registered with the given name.
         * If no gamemode was found, returns nullptr.
         */
        virtual std::unique_ptr<IGameMode> CreateGameMode(const std::string& a_Name) = 0;

        /*
         * Get a reference to the voxel registry.
         */
        virtual VoxelRegistry& GetVoxelRegistry() = 0;

        /*
         * Get the logger for the server.
         */
        virtual utilities::Logger& GetLogger() = 0;

        /*
         * Get a const reference to the server settings.
         */
        virtual const ServerSettings& GetServerSettings() const = 0;

        /*
         * Get a reference to the servers connection manager.
         */
        virtual IConnectionManager& GetConnectionManager() = 0;
    };
}
