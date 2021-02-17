#pragma once
#include <IChunkStore.h>

namespace voxl
{
    class ClientChunkStore : public IChunkStore
    {
    public:
        IChunk* GetChunk(const glm::ivec3& a_Coordinates) override;
        IChunk* LoadChunk(std::unique_ptr<IChunk>&& a_Chunk) override;
        bool UnloadChunk(const glm::ivec3& a_Coordinates) override;
        void UnloadAll() override;
        size_t GetNumLoadedChunks() override;
        iterator begin() override;
        iterator end() override;
        const_iterator begin() const override;
        const_iterator end() const override;
    };
}


