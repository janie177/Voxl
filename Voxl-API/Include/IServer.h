#pragma once
#include <memory>
#include <string>
#include <vector>

#include "logging/Logger.h"

#define GAMEMODES_FOLDER "gamemodes"

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

    struct GameSettings;

    enum class ServerState
    {
        STARTING,
        RUNNING,
        SHUT_DOWN,
    };

    struct ServerSettings
    {
        //The ticks per second at which the server processes information.
        std::uint32_t tps = 32;

        //A list of games this server hosts.
        //The first game is default for now.
        std::vector<std::string> games{"default"};

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
        virtual ServerState GetState() = 0;

        /*
         * Initialize server systems. 
         */
        virtual void Start() = 0;

        /*
         * Stop the server.
         */
        virtual void ShutDown(bool a_SaveAll) = 0;

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
         * Register a world generator with the given name.
         */
        virtual void RegisterWorldGenerator(const std::string& a_Name, std::shared_ptr<IWorldGenerator>& a_Generator) = 0;

        /*
         * Register a gamemode with the server using the given name as handle.
         * This instance can be assigned to worlds using that name.
         *
         */
        virtual void RegisterGameMode(const std::string& a_Name, std::shared_ptr<IGameMode>& a_GameMode) = 0;

        /*
         * Get the world generator registered with the given name.
         * If no world generator exists with that name, nullptr is returned.
         */
        virtual std::shared_ptr<IWorldGenerator> GetWorldGenerator(const std::string& a_Name) = 0;

        /*
         * Create a new instacne of the gamemode registered with the given name.
         * If no gamemode was registered with that name, return nullptr.
         */
        virtual std::shared_ptr<IGameMode> CreateGamemode(const std::string& a_Name) = 0;

        /*
         * Create a new game from the given settings.
         */
        virtual std::shared_ptr<IGame> CreateGame(const GameSettings& a_Settings) = 0;

        /*
         * Load the game with the given name.
         */
        virtual bool LoadGame(const std::string& a_Name) = 0;

        /*
         * Unload the game with the given name.
         */
        virtual bool UnloadGame(const std::string& a_Name) = 0;

        /*
         * Delete the game with the given name forever.
         */
        virtual bool DeleteGame(const std::string& a_Name) = 0;

        /*
         * Get all currently active games.
         */
        virtual std::vector<std::shared_ptr<IGame>> GetGames() = 0;

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
