#include "ClientChunk.h"

namespace voxl
{
    glm::ivec3 ClientChunk::GetChunkCoordinates()
    {
        //TODO
        return glm::ivec3(0);
    }

    ChunkState ClientChunk::GetState()
    {
        //TODO
        return ChunkState::LOADING;
    }

    VoxelData* ClientChunk::GetVoxelData()
    {
        //TODO
        return nullptr;
    }

    VoxelData const* ClientChunk::GetVoxelData() const
    {
        //TODO
        return nullptr;
    }

    void ClientChunk::Save(IWorld& a_World)
    {
    }

    void ClientChunk::Unload(IWorld& a_World)
    {

    }

    bool ClientChunk::IsDirty()
    {
        //TODO
        return false;
    }

    void ClientChunk::SetDirty(bool a_Dirty)
    {
        //TODO
    }
}
