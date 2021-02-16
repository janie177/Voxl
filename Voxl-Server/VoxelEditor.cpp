#include "VoxelEditor.h"

namespace voxl
{
    void VoxelEditor::QueueUpdates(const glm::ivec3& a_Start, const glm::ivec3& a_End,
        const std::function<bool(const glm::ivec3&, VoxelData&)>& a_Function)
    {
        //TODO
    }

    void VoxelEditor::QueueUpdate(const glm::ivec3& a_Position, const VoxelData& a_Data)
    {
        //TODO
    }

    void VoxelEditor::ApplyPendingChanges(IChunkStore& a_ChunkStore)
    {
        //TODO

    }
}

