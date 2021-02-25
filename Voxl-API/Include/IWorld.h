#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "IEntity.h"

namespace voxl
{
    enum class EntityType;
    class IServer;
    class IVoxelEditor;
    class IWorldGenerator;
    class IGameMode;
    class IChunkStore;
    class IClientConnection;

    /*
     * Current status of the world.
     */
    enum class WorldState
    {
        LOADING,
        RUNNING,
        SAVING,
        UNLOADED
    };

    struct WorldSettings
    {
        std::string name = "world";                 //The name of the world (used to identify and lookup).
        std::uint64_t seed = 0;                     //Seed used for world generation.
        std::string generator = "default";          //World generator name.
        std::uint32_t renderDistance = 10;          //The radius around players at which chunks should load.
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

        /*
         * Add a new player to the world.
         * This adds the player object right away and can cause iterator invalidation.
         * This should never be called during an active game loop.
         *
         * Returns a handle to the player after being inserted.
         */
        virtual IPlayer& AddPlayer(std::unique_ptr<IPlayer>&& a_Player) = 0;

        /*
         * Add a new entity to the world.
         * The entity is added right away.
         * This can cause iterator invalidation and should never be called during an active game loop.
         *
         * Returns a handle to the entity after being inserted.
         */
        virtual IEntity& AddEntity(std::unique_ptr<IEntity>&& a_Entity) = 0;

        /*
         * Queue an entity for spawn next game cycle.
         * This delays the spawn and can be safely called from within the game cycle.
         */
        virtual void QueueEntityAdd(std::unique_ptr<IEntity>&& a_Entity) = 0;

        /*
         * Get the entity with the given ID.
         * If no entity with that ID exists, returns nullptr.
         */
        virtual IEntity* GetEntity(std::uint64_t a_Id) = 0;

        /*
         * Get the player with the given ID.
         * If no player with that ID exists, returns nullptr.
         */
        virtual IPlayer* GetPlayer(std::uint64_t a_Id) = 0;

        /*
         * Get the state of this world.
         */
        virtual WorldState GetWorldState() const = 0;
    };
}
