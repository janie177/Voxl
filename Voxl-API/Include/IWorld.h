#pragma once
#include <memory>
#include <string>

namespace voxl
{
    class IVoxelEditor;
    class IWorldGenerator;
    class IGameMode;
    class IChunkStore;

    struct WorldSettings
    {
        std::uint64_t seed;     //Seed used for world generation.
        std::string generator;  //World generator name.
        std::string gameMode;   //The gamemode name.
    };

    class IWorld
    {
    public:
        /*
         * Save the world to file.
         */
        virtual void Save(const WorldSettings& a_Settings) = 0;

        /*
         * Load the world from file.
         */
        virtual void Load(const WorldSettings& a_Settings) = 0;

        /*
         * Update the world.
         */
        virtual void Tick(double a_DeltaTime) = 0;

        /*
         * Set the worlds gamemode.
         */
        virtual void SetGameMode(std::unique_ptr<IGameMode>&& a_GameMode) = 0;

        /*
         * Set the world generator that this world should use.
         */
        virtual void SetWorldGenerator(std::shared_ptr<IWorldGenerator>& a_Generator);

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
    };
}
