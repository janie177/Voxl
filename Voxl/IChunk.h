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
        /*
         * Get the Chunks X, Y and Z coordinates in the world.
         */
        virtual glm::ivec3 GetChunkCoordinates() = 0;

        /*
         * Returns true when the chunk has finished loading or if something is still happening to it.
         */
        virtual ChunkState GetState() = 0;

        /*
         * Get a pointer to the array of voxels in this chunk.
         */
        virtual VoxelData* GetVoxelData() = 0;

        /*
         * Save the chunk for the given world.
         */
        virtual void Save(IWorld* a_World) = 0;

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
