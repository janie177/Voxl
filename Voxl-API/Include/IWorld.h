#pragma once
#include <memory>
#include <string>

namespace voxl
{
    class IServer;
    class IVoxelEditor;
    class IWorldGenerator;
    class IGameMode;
    class IChunkStore;

    struct WorldSettings
    {
        std::string name = "world";                 //The name of the world (used to identify and lookup).
        std::string playersDirectory = "players";   //The directory to save player files in. Relative to server root.
        std::uint64_t seed = 0;                     //Seed used for world generation.
        std::string generator = "default";          //World generator name.
        std::string gameMode = "default";           //The gamemode name.
        bool enableSaving = true;                   //True if saving should be enabled.
    };

    class IWorld
    {
    public:
        virtual ~IWorld() = default;

        /*
         * Get the name of this world.
         */
        virtual const std::string& GetName() = 0;

        /*
         * Save the world to file.
         */
        virtual void Save() = 0;

        /*
         * Unload the world from memory.
         * Delete systems and free memory here, as well as ensure players don't get stuck.
         */
        virtual void Unload() = 0;

        /*
         * Load the world from file.
         */
        virtual bool Load(IServer& a_Server) = 0;

        /*
         * Update the world.
         */
        virtual void Tick(double a_DeltaTime) = 0;

        /*
         * Set the worlds gamemode.
         * This saves and then disables the previous gamemode.
         * The new gamemode is then loaded.
         */
        virtual void SetGameMode(std::unique_ptr<IGameMode>&& a_GameMode) = 0;

        /*
         * Set the world generator that this world should use.
         */
        virtual void SetWorldGenerator(std::shared_ptr<IWorldGenerator>& a_Generator) = 0;

        /*
         * Get the world generator used by this world.
         */
        virtual IWorldGenerator& GetWorldGenerator() = 0;

        /*
         * Get the worlds gamemode.
         */
        virtual IGameMode& GetGameMode() = 0;

        /*
         * Get the chunk store for this world.
         */
        virtual IChunkStore& GetChunkStore() = 0;

        /*
         * Get the voxel editor used to modify the world.
         */
        virtual IVoxelEditor& GetVoxelEditor() = 0;

        /*
         * Get the settings for this world.
         */
        virtual const WorldSettings& GetSettings() const = 0;

        /*
         * Save the given world settings file for this world.
         */
        virtual void SaveWorldSettings(const WorldSettings& a_Settings) = 0;
    };
}
