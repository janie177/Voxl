#pragma once
#include <functional>
#include <glm/glm.hpp>

namespace voxl
{
    struct VoxelData;
    class IChunkStore;

    /*
     * VoxelEditor allows the world to be changed.
     */
    class IVoxelEditor
    {
    public:
        virtual ~IVoxelEditor() = default;

        /*
         * Queue multiple voxel updates in a region using a custom function.
         * The function provided returns true when a change was made. False otherwise.
         * When false is returned, no changes are queued for that particular voxel.
         */
        virtual void QueueUpdates(const glm::ivec3& a_Start, const glm::ivec3& a_End, const std::function<bool(const glm::ivec3&, VoxelData&)>& a_Function) = 0;

        /*
         * Queue a voxel update.
         */
        virtual void QueueUpdate(const glm::ivec3& a_Position, const VoxelData& a_Data) = 0;

        /*
         * Apply all pending changes to the chunks in the given ChunkStore.
         * Updates to chunks that aren't loaded will cause the chunk to be loaded and altered.
         * The chunk is then saved again afterward.
         */
        virtual void ApplyPendingChanges(IChunkStore& a_ChunkStore) = 0;
    };
}
