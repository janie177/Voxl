#pragma once
#include <IChunkMesh.h>

namespace voxl
{
    class ChunkMesh : public IChunkMesh
    {
    public:
        bool IsLoaded() override;
        void Generate(IChunk* a_Chunk) override;
        ChunkMeshState GetState() override;
        void Destroy() override;
    };
}


