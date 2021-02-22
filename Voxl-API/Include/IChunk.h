#pragma once
#include <glm/glm.hpp>

#include "VoxelData.h"
#include "Utility.h"

namespace voxl
{
    class IWorld;

    enum class ChunkState
    {
        LOADING,
        GENERATING,
        POPULATING,
        READY
    };

    /*
     * Interface detailing a chunk containing voxel data.
     * The size of a chunk is defined in Utility.h which also offers some utility functions to access the data in a chunk.
     */
    class IChunk
    {
    public:
        virtual ~IChunk() = default;
        /*
         * Get the Chunks X, Y and Z coordinates in the world.
         */
        virtual glm::ivec3 GetChunkCoordinates() = 0;

        /*
         * Tick this chunk.
         */
        virtual void Tick(float a_DeltaTime) = 0;

        /*
         * Returns the current state of the chunk.
         */
        virtual ChunkState GetState() = 0;

        /*
         * Set the state of the chunk.
         */
        virtual void SetState(ChunkState a_State) = 0;

        /*
         * Get a pointer to the array of voxels in this chunk.
         */
        virtual VoxelData* GetVoxelData() = 0;

        /*
         * Get a const pointer to the array of voxels in this chunk.
         */
        virtual VoxelData const* GetVoxelData() const = 0;

        /*
         * Save the chunk for the given world.
         */
        virtual void Save(IWorld& a_World) = 0;

        /*
         * Perform operations when the chunk is unloaded from memory.
         */
        virtual void Unload(IWorld& a_World) = 0;

        /*
         * Returns true when this chunk has been changed and requires synchronization.
         */
        virtual bool IsDirty() = 0;

        /*
         * Mark this chunk as dirty so that it will be synchronized.
         */
        virtual void SetDirty(bool a_Dirty) = 0;
    };
}
