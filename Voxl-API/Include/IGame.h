#pragma once
#include <memory>
#include <string>
#include <vector>

namespace voxl
{
    class IClientConnection;
    struct WorldSettings;
    class IGameMode;
    class IWorld;

    struct GameSettings
    {
        //The name of this game. Used for loading and saving paths.
        std::string gameName;

        //The name of the gamemode used by this game (as registered in the server).
        std::string gameModeName;
    };

    /*
     * IGame contains a game-mode and one or more worlds.
     * A server can contain multiple games.
     */
    class IGame
    {
    public:
        virtual ~IGame() = default;

        /*
         * Retrieve the gamemode used for this game instance.
         */
        virtual std::shared_ptr<IGameMode> GetGameMode() = 0;

        /*
         * Tick this game.
         */
        virtual void Tick(float a_DeltaTime) = 0;

        /*
         * Save this game and everything in it.
         */
        virtual void Save() = 0;

        /*
         * Unload this game and everything in it.
         */
        virtual void Unload() = 0;

        /*
         * Get the name of this game.
         */
        virtual std::string GetName() = 0;

        /*
         * Load this game with the given settings.
         */
        virtual bool Load() = 0;

        /*
         * Load the settings file into the given settings folder.
         * Returns true if the game was successfully loaded.
         * False otherwise.
         */
        virtual bool LoadSettingsFile(GameSettings& a_Settings) = 0;

        /*
         * Create a game settings file for the given settings.
         */
        virtual bool CreateSettingsFile(const GameSettings& a_Settings) = 0;

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
         * Add the given connection to the game.
         */
        virtual void AddPlayer(IClientConnection& a_Connection) = 0;

        /*
         * Remove the given connection from the game.
         */
        virtual void RemovePlayer(IClientConnection& a_Connection) = 0;

        /*
         * Get all worlds that are currently loaded.
         */
        virtual std::vector<IWorld*> GetWorlds() = 0;
    };
}
