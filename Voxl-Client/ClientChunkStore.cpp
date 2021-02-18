#include "ClientChunkStore.h"

namespace voxl
{
    IChunk* ClientChunkStore::GetChunk(const glm::ivec3& a_Coordinates)
    {
        //TODO
        return nullptr;
    }

    IChunk* ClientChunkStore::LoadChunk(std::unique_ptr<IChunk>&& a_Chunk)
    {
        //TODO
        return nullptr;
    }

    bool ClientChunkStore::UnloadChunk(const glm::ivec3& a_Coordinates)
    {
        //TODO
        return false;
    }

    void ClientChunkStore::UnloadAll()
    {

    }

    size_t ClientChunkStore::GetNumLoadedChunks()
    {
        //TODO
        return 0;
    }

    IChunkStore::iterator ClientChunkStore::begin()
    {
        //TODO
        return nullptr;
    }

    IChunkStore::iterator ClientChunkStore::end()
    {
        //TODO
        return nullptr;
    }

    IChunkStore::const_iterator ClientChunkStore::begin() const
    {
        //TODO
        return nullptr;
    }

    IChunkStore::const_iterator ClientChunkStore::end() const
    {
        //TODO
        return nullptr;
    }
}
