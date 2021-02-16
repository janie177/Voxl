#include "ChunkStore.h"

namespace voxl
{
    IChunk* ChunkStore::GetChunk(const glm::ivec3& a_Coordinates)
    {
        //TODO
        return nullptr;
    }

    IChunk* ChunkStore::LoadChunk(std::unique_ptr<IChunk>&& a_Chunk)
    {
        //TODO
        return nullptr;
    }

    bool ChunkStore::UnloadChunk(const glm::ivec3& a_Coordinates)
    {
        //TODO
        return false;
    }

    void ChunkStore::UnloadAll()
    {
        //TODO
    }

    size_t ChunkStore::GetNumLoadedChunks()
    {
        //TODO
        return 0;
    }

    IChunkStore::iterator ChunkStore::begin()
    {
        //TODO
        return nullptr;
    }

    IChunkStore::iterator ChunkStore::end()
    {
        //TODO
        return nullptr;
    }

    IChunkStore::const_iterator ChunkStore::begin() const
    {
        //TODO
        return nullptr;
    }

    IChunkStore::const_iterator ChunkStore::end() const
    {
        //TODO
        return nullptr;
    }
}
