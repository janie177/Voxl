#pragma once
#include <IChunk.h>

namespace voxl
{
    class ClientChunk : public IChunk
    {
    public:
        glm::ivec3 GetChunkCoordinates() override;
        ChunkState GetState() override;
        VoxelData* GetVoxelData() override;
        VoxelData const* GetVoxelData() const override;
        void Save(IWorld& a_World) override;
        void Unload(IWorld& a_World) override;
        bool IsDirty() override;
        void SetDirty(bool a_Dirty) override;
    };
}


