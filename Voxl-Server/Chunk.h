#pragma once

#include <IChunk.h>
#include <VoxelData.h>

namespace voxl
{
    class Chunk : public IChunk
    {
    public:
        explicit Chunk(const glm::ivec3& a_Coordinates);

        glm::ivec3 GetChunkCoordinates() override;
        ChunkState GetState() override;
        VoxelData* GetVoxelData() override;
        VoxelData const* GetVoxelData() const override;
        void Save(IWorld& a_World) override;
        void Unload(IWorld& a_World) override;
        bool IsDirty() override;
        void SetDirty(bool a_Dirty) override;

    private:
        VoxelData m_Data[CHUNK_SIZE_CUBED];
        glm::ivec3 m_Coordinates;
        ChunkState m_State;
        bool m_Dirty;
    };
}