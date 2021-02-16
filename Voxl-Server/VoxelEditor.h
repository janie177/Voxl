#pragma once
#include <IVoxelEditor.h>

namespace voxl
{
    class VoxelEditor : public IVoxelEditor
    {
    public:
        void QueueUpdates(const glm::ivec3& a_Start, const glm::ivec3& a_End, const std::function<bool(const glm::ivec3&, VoxelData&)>& a_Function) override;
        void QueueUpdate(const glm::ivec3& a_Position, const VoxelData& a_Data) override;
        void ApplyPendingChanges(IChunkStore& a_ChunkStore) override;
    };
}