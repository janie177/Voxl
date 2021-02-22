#include "Chunk.h"

namespace voxl
{
    Chunk::Chunk(const glm::ivec3& a_Coordinates) : m_Coordinates(a_Coordinates)
    {
        
    }

    glm::ivec3 Chunk::GetChunkCoordinates()
    {
        return m_Coordinates;
    }

    ChunkState Chunk::GetState()
    {
        return m_State;
    }

    VoxelData* Chunk::GetVoxelData()
    {
        return m_Data;
    }

    VoxelData const* Chunk::GetVoxelData() const
    {
        return m_Data;
    }

    void Chunk::Save(IWorld& a_World)
    {

    }

    void Chunk::Unload(IWorld& a_World)
    {
        //TODO
    }

    bool Chunk::IsDirty()
    {
        return m_Dirty;
    }

    void Chunk::SetDirty(bool a_Dirty)
    {
        m_Dirty = a_Dirty;
    }
}
