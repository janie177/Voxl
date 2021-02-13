#pragma once
#include <memory>
#include <string>
#include <vector>

namespace voxl
{
    struct WorldSettings;
    class IWorldGenerator;
    class IWorld;
    class IGameMode;
    class IChunkStore;
    class VoxelRegistry;
    class IClientConnection;

    class IServer
    {
    public:
        virtual ~IServer() = 0;

        /*
         * Initialize server systems.
         */
        virtual void Init() = 0;

        /*
         * Update the server, ticking and updating all worlds and game-modes.
         */
        virtual void Tick(double a_DeltaTime) = 0;

        /*
         * Ge the world with the given name.
         */
        virtual IWorld& GetWorld(const std::string& a_Name) = 0;

        /*
         * Load a world from file.
         */
        virtual IWorld& LoadWorld(const std::string& a_Name) = 0;

        /*
         * Unload a world from memory.
         */
        virtual void UnloadWorld(IWorld& a_World) = 0;

        /*
         * Save a world to file.
         */
        virtual void SaveWorld(IWorld* a_World) = 0;

        /*
         * Register a world generator with the given name.
         */
        virtual void RegisterWorldGenerator(const std::string& a_Name, std::shared_ptr<IWorldGenerator>& a_Generator) = 0;

        /*
         * Register a gamemode with the server using the given name as handle.
         */
        virtual void RegisterGameMode(const std::string& a_Name, std::unique_ptr<IGameMode>&& a_GameMode);

        /*
         * Get a reference to the voxel registry.
         */
        virtual VoxelRegistry& GetVoxelRegistry() = 0;

        /*
         * Get all currently connected clients.
         */
        virtual std::vector<IClientConnection*>& GetConnectedClients() = 0;

        /*
         * Process all connected clients.
         */
        virtual void ProcessClientConnections() = 0;

        /*
         * Called when a client connects to the server.
         */
        virtual void OnClientConnect() = 0;
    };
}
